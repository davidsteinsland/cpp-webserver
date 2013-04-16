#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP 

#include "webserver.h"
#include "../config.h"
#include "../net/socket.h"
#include "../http/response.h"
#include "../http/request.h"
#include "../utils/fileutils.h"

#ifdef TARGET_OS_MAC
	#error Mac is not supported yet
#endif

#ifdef __linux__
	#include "../net/unix/socket.cpp"
	#include "../net/unix/clientsocket.cpp"
	#include "unix/module.cpp"
#endif

#ifdef _WIN32
	#include "../net/win32/socket.cpp"
	#include "../net/win32/clientsocket.cpp"
	#include "win32/module.cpp"
#endif

#include <string>
#include <iostream>

webserver::webserver::webserver(int p)
{
	port = p;
	listenSocket = new net::socket();
}

webserver::webserver::~webserver()
{
	listenSocket->close();
	delete listenSocket;
}

int webserver::webserver::listen ()
{
	if (listenSocket->listen(port) != 1)
	{
		return 0;
	}
	
	while (true)
	{
		net::clientsocket* client = listenSocket->accept();
		
		if ( client == NULL )
		{
			delete client;
			continue;
		}
		
		http::request* request = new http::request(client->recieve());
		
		/**
		 * Try to load the following files, in prioritized order:
		 * 1. The requested file
		 * 2. Directory index
		 * 2.1 301 page if directory exist, but no index.html
		 * 3. 404 page
		 * 3.1 if requested file is a directory, it is possibly dynamic content
		 */
		
		http::response* response = new http::response (200, "text/plain");
		std::string filename = config::HTML_ROOT + request->uri();
		
		bool dynamic_request = false;
		
		if ( utils::fileutils::is_file (config::MODULES_ROOT + request->uri() + config::MODULE_EXT))
		{
			dynamic_request = true;
			filename = config::MODULES_ROOT + request->uri() + config::MODULE_EXT;
		}
		else if ( !utils::fileutils::is_file (filename) )
		{
			if ( utils::fileutils::is_directory (filename))
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
			else
			{
				filename = config::HTML_ROOT + "/404.html";
				response->set_status(404);
			}
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
				std::string content_type = utils::fileutils::content_type (filename.substr(filename.find_last_of(".")));
				
				response->set_content_type (content_type);
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
		 * static request: a file or a directory
		 *
		 * debug data:
		 */
		std::string contents = response->body();
		contents.append("Request line: " + request->status_line() + "<br />\n");
		contents.append("URL: " + request->url() + "<br />\n");
		contents.append("URI: " + request->uri() + "<br />\n");
		contents.append("QS: " + request->query_string() + "<br />\n");
		contents.append("Filename: " + filename + "<br />\n");
		
		response->set_body(contents);
		
		/**
		 * write response to client
		 */
		client->send(response);
		client->close();
		
		delete client;
		delete request;
		delete response;
	}
	
	return 1;
}

#endif