#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP 

#include "config.h"
#include "webserver/webserver.h"
#include "webserver/module.h"
#include "http/response.h"
#include "http/request.h"
#include "net/socket.h"
#include "net/clientsocket.h"
#include "utils/fileutils.h"

#include <string>
#include <iostream>

#include <queue>
#include <deque>
#include <pthread.h>
#include <unistd.h>

std::deque<pthread_t> workers;
pthread_cond_t condition;
pthread_mutex_t mutex;

webserver::webserver::webserver(int p)
{
	port = p;
	listenSocket = new net::socket();
	
	pthread_cond_init(&condition, 0);
	pthread_mutex_init(&mutex, 0);
}

webserver::webserver::~webserver()
{
	listenSocket->close();
	delete listenSocket;
}

void webserver::webserver::shutdown()
{
	started = false;
}

int webserver::webserver::listen ()
{
	int error;
	if ( (error = listenSocket->listen(port)) != 0)
	{
		return error;
	}
	
	started = true;
	
	while (started)
	{
		net::clientsocket* client = listenSocket->accept();
		
		if ( client == NULL )
		{
			delete client;
			continue;
		}
		
		pthread_mutex_lock(&mutex);

		// queue is full, so wait for consumer
		while (workers.size() == config::NUM_WORKER_THREADS)
			pthread_cond_wait(&condition, &mutex);

		// when we get here, the queue has space
		pthread_t pid;
		pthread_create(&pid, NULL, handle_request, client);
		workers.push_back(pid);
		
		// make sure we wake a sleeping consumer
		if (workers.size() == 1)
			pthread_cond_signal(&condition);

		pthread_mutex_unlock(&mutex);
	}
	
	return 0;
}

void *webserver::handle_request (void *c)
{
	pthread_mutex_lock(&mutex);

	// wait for something to do
	while (workers.size() == 0)
		pthread_cond_wait(&condition, &mutex);

	// if we get here, we have some work
	pthread_t pid = workers.front();
	net::clientsocket* client = (net::clientsocket*) c;
	
	if (client == NULL || !client->valid())
	{
		return 0;
	}
	
	// make sure we wake a sleeping producer
	if (workers.size() == 1)
		pthread_cond_signal(&condition);

	workers.pop_front();
	pthread_mutex_unlock(&mutex);
	
	// handle the request
	http::request* request = http::request::parse(client->recieve());
	
	/**
	 * Try to load the following files, in prioritized order:
	 * 1. The requested file
	 * 2. Directory index
	 * 2.1 301 page if directory exist, but no index.html
	 * 3. 404 page
	 * 3.1 if requested file is a directory, it is possibly dynamic content
	 */
	
	http::response* response = new http::response (200, "text/plain");
	
	if (request == NULL)
	{
		// invalid request; 400 Bad Request
		response->set_status(400);
		response->set_body ("Invalid Request");
	}
	else
	{
		std::string filename = config::HTML_ROOT + request->uri();
		bool dynamic_request = false;
		
		
		if ( utils::fileutils::is_file (config::MODULES_ROOT + request->uri() + config::MODULE_EXT))
		{
			dynamic_request = true;
			filename = config::MODULES_ROOT + request->uri() + config::MODULE_EXT;
		}
		else if ( utils::fileutils::is_directory (filename))
		{
			if ( utils::fileutils::is_file (filename + "index.html") )
			{
				filename = filename + "index.html";
			}
			else
			{
				filename = config::HTML_ROOT + "/301.html";
				response->set_status(301);
			}
		}
		else if ( !utils::fileutils::is_file (filename) )
		{
			filename = config::HTML_ROOT + "/404.html";
			response->set_status(404);
		}
		
		try
		{
			if (dynamic_request)
			{
				/**
				 * dynamic request; file requested path is a file, but has no extension (executable program).
				 * Contact program with the method:
				 *
				 * handle_request (request, &response);
				 * 
				 * "request" now contains the data to be sent to the client.
				 */
				try
				{
					module* m = new module (filename.c_str());
					void* initializer = m->call ("handle_request");
					
					// cast initializer to its proper type and use
					typedef int (*handle_request_type)(http::request*,http::response*);
					handle_request_type init_func = (handle_request_type) initializer;
					
					// call dynamic page
					init_func (request, response);
					
					delete m;
				}
				catch (char const* c)
				{
					std::cerr << c << std::endl;
				}
			}
			else
			{
				unsigned ext_pos = filename.find_last_of(".");
				if (ext_pos == std::string::npos)
					response->set_content_type("text/plain");
				else
					response->set_content_type (utils::fileutils::content_type (filename.substr(ext_pos), "text/plain"));
				response->set_body (utils::fileutils::contents(filename));
			}
		}
		catch (...)
		{
			// In case 301.html or 404.html does not exist, we'd have to return a generic error
			response->set_content_type("");
			response->set_status(404);
		}
		
		/**
		 * debug data
		 */
		/*std::string contents = response->body();
		contents.append("Request line: " + request->status_line() + "<br />\n");
		contents.append("URL: " + request->url() + "<br />\n");
		contents.append("URI: " + request->uri() + "<br />\n");
		contents.append("QS: " + request->query_string() + "<br />\n");
		contents.append("Filename: " + filename + "<br />\n");
		
		response->set_body(contents);*/
	}
	
	
	/**
	 * write response to client
	 */
	client->send(response);
	client->close();
	
	delete client;
	delete request;
	delete response;
	
	pthread_exit(NULL);
	
	return 0;
}

#endif
