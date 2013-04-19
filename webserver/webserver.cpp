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

// queue of requests; to be picked up by the worker threads
std::deque<net::clientsocket*> requests_queue;

pthread_cond_t condition;
pthread_mutex_t mutex;
// the pids of the worker threads
pthread_t worker_pids[config::NUM_WORKER_THREADS];

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
		
	// starting worker threads
	for (unsigned int i = 0; i < config::NUM_WORKER_THREADS; i++)
	{
		pthread_create(&worker_pids[i], NULL, worker_thread, &i);
	}
	
	while (listenSocket->active())
	{
		net::clientsocket* client = listenSocket->accept();
		
		if ( client == NULL || ! client->valid() )
		{
			delete client;
			continue;
		}
		
		pthread_mutex_lock(&mutex);
		
		requests_queue.push_back(client);
		
		pthread_cond_signal(&condition);
		pthread_mutex_unlock(&mutex);
	}
	
	return 0;
}

void *webserver::worker_thread (void *a)
{
	int worker_id = *((int*)a);

	while (true)
	{
		// wait for something to do
		pthread_mutex_lock(&mutex);
		while (requests_queue.size() == 0)
			pthread_cond_wait(&condition, &mutex);

		// if we get here, we have some work
		net::clientsocket* client = requests_queue.front();
		requests_queue.pop_front();
		// unlock mutex so other worker threads may access the queue
		pthread_mutex_unlock(&mutex);
		
		// handle the request
		handle_request(client);
	}
	
	pthread_exit(NULL);
	
	return 0;
}

void webserver::handle_request (net::clientsocket* client)
{
	char *buf = new char[512];
	int k = client->recieve(buf, 512);
	
	if ( k <= 0 )
	{
		delete client;
		delete [] buf;
		return;
	}
	
	http::request* request = http::request::parse(std::string(buf, 0, k));
	http::response* response = new http::response (200, "text/plain");
	
	delete [] buf;
	
	/**
	 * Try to load the following files, in prioritized order:
	 * 1. a module file
	 * 1. The requested file
	 * 2. Directory index
	 * 2.1 301 page if directory exist, but no index.html
	 * 3. 404 page
	 */
	if (request == NULL)
	{
		// invalid request; 400 Bad Request
		response->set_status(400);
		response->set_body ("Invalid Request");
	}
	else if (request->method() != "POST" && request->method() != "GET")
	{
		response->set_status(501);
		response->set_body ("Method Not Implemented: This server support POST and GET only.");
	}
	else if ( utils::fileutils::is_file (config::MODULES_ROOT + request->file() + config::MODULE_EXT))
	{
		if (! load_module (request, response))
		{
			response->set_status(500);
			response->set_body ("Could not load or call the requested module");
		}
	}
	else
	{
		if ( ! load_file (request, response))
		{
			// In case 301.html or 404.html does not exist, we'd have to return a generic error
			response->set_content_type("");
			response->set_status(404);
			response->set_body ("The requested path is not found");
		}
	}
	
	/**
	 * write response to client and exit thread
	 */
	client->send(response);
	client->close();
	
	delete client;
	delete request;
	delete response;
}

bool webserver::load_module (http::request* request, http::response* response)
{
	std::string filename = config::MODULES_ROOT + request->file() + config::MODULE_EXT;
	
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
		return false;
	}
	
	return true;
}

bool webserver::load_file (http::request* request, http::response* response)
{
	std::string filename = config::HTML_ROOT + request->file();
		
	if ( utils::fileutils::is_file (filename) )
	{
		/**	
		 * filename is a file
		 * @TODO: Implements File Handlers here
		 */
	}
	else if ( utils::fileutils::is_directory (filename))
	{
		/**
		 * Send a 301 Header if request points to a directory, but the URI doesn't ends with a forward slash, /.
		 */
		if ( *filename.rbegin() != '/' )
		{
			response->set_status(302);
			response->set_header ("Location", request->url() + request->uri() + "/");
			
			return true;
		}
		
		if ( utils::fileutils::is_file (filename + "index.html") )
		{
			filename = filename + "index.html";
		}
		else if ( utils::fileutils::is_file (filename + "index.htm") )
		{
			filename = filename + "index.htm";
		}
		else
		{
			filename = config::HTML_ROOT + "/301.html";
			response->set_status(301);
		}
	}
	else
	{
		filename = config::HTML_ROOT + "/404.html";
		response->set_status(404);
	}
	
	try
	{
		/**
		 * Find mime type based on file extension,
		 * and default to text/plain if no mime can be found
		 */
		unsigned ext_pos = filename.find_last_of(".");
		if (ext_pos != std::string::npos && ext_pos <= filename.length())
			response->set_content_type (utils::fileutils::content_type (filename.substr(ext_pos), "text/plain"));
		
		// load contents of file
		response->set_body (utils::fileutils::contents(filename));
	}
	catch (...)
	{
		return false;
	}
	
	return true;
}

#endif