#ifndef WORKER_CPP
#define WORKER_CPP

#include "config.h"
#include "webserver/worker.h"
#include "webserver/worker_pool.h"
#include "webserver/module.h"
#include "utils/fileutils.h"
#include "http/response.h"
#include "http/request.h"
#include "net/clientsocket.h"

#include <string>
#include <sstream>
#include <iostream>
#include <deque>

void webserver::worker_thread (concurrency::thread* t)
{
	//worker_pool<net::clientsocket*>* pool = (worker_pool<net::clientsocket*>*)arg;
	worker_pool<net::clientsocket*>* pool = static_cast<worker_pool<net::clientsocket*>*> (t->get_args());

	while (pool->active())
	{
		net::clientsocket* client = pool->get_job();
		
		/*
		 * This loop makes the browser hang..
		 * The server recieves all the data, but the connection is not closing
		 * from the browser, making the web server listening for more data.
		 */
		// /*
		std::string request_content;
		
		int buflen = 128;
		char buf[buflen];
		int bytesRead = 0;
		int recvResult = 0;
		bool error = false;
		
		do
		{
			recvResult = client->recieve(buf, buflen);
			if ( recvResult > 0 )
			{
				bytesRead += recvResult;
				
				request_content.append (std::string(buf, 0, bytesRead));
				
				/*
				 * 1. Check for CRLF + CRLF
				 *   1.1 If found, check request_content for any Content-Length header
				 *   1.1.1 If found, read additional X bytes
				 *  1.2 If not found, end request
				 * 2. Continue with reading
				 */
				unsigned headers_end_pos = request_content.find("\r\n\r\n");
				if ( headers_end_pos == std::string::npos )
					continue;
				
				unsigned content_length_pos = request_content.find ("Content-Length:");
				if ( content_length_pos == std::string::npos)
					break;
				/*
					makes sure we haven't read the content body yet.
					we know how large the content-body is, by content-length.
						content_body_s = int(content-length);
					we can also find the size of the headers:
						headers_s = request_content.substr(0, pos(\r\n\r\n)).length();
					we then have the following equation:
						request_s = content_body_s + headers_s
					we know also how many bytes we have read already (bytesRead):
						remaining_s = request_s - bytesRead
									= content_body_s + headers_s - bytesRead
				*/
				std::string headers = request_content.substr(0, headers_end_pos);
				
				std::istringstream fstr (request_content.substr (content_length_pos));
				std::string content_length_header;
				std::getline ( fstr, content_length_header );
				
				int headers_length = headers.length();
				int content_length = atoi(content_length_header.substr (16).c_str());
				
				int remainingBytes = headers_length + content_length - bytesRead;
				if (remainingBytes <= 0)
					break;
			}
			else if ( recvResult == 0 )
			{
				// std::cout << "Connection closed\n" << std::endl;
			}
			else
			{
				std::cerr << "Recv failed: " << ERRNO << std::endl;
				std::cerr << strerror (ERRNO) << std::endl;
				error = true;
			}
		} while( recvResult > 0 );
		
		if (error)
		{
			delete client;
			continue;
		}
		
		// */
		
		/*
		char* buf = new char[512];
		int k = client->recieve(buf, 512);
		
		if ( k <= 0 )
		{
			delete client;
			delete [] buf;
			
			std::cerr << "Recieve failed with code #" << ERRNO << std::endl;
			std::cerr << strerror (ERRNO) << std::endl;
			
			continue;
		}
		
		std::string request_content = std::string(buf, 0, k);
		*/
		
		http::request* request = http::request::parse(request_content);
		http::response* response = new http::response (200, "text/plain");
		
		if (request == NULL)
		{
			// invalid request; 400 Bad Request
			response->set_status(400);
			response->set_body ("Invalid Request");
		}
		
		if (request->method() != "POST" && request->method() != "GET")
		{
			response->set_status(501);
			response->set_body ("Method Not Implemented: This server support POST and GET only.");
		}
		else
		{
			// handle the request
			handle_request(request, response);
		}
		
		if (response->code() != 200)
		{
			/**
			 * 1. Try to load an error file from errors/ folder
			 * 2. Revert to a generic error if that could not be done
			 */
			try
			{
				std::stringstream ss;
				ss << config::ERRORS_ROOT << "/" << response->code() << ".html";
				
				response->set_body (utils::fileutils::contents(ss.str()));
				response->set_content_type ("text/html");
				
			}
			catch (int e)
			{
				response->set_content_type("");
				// could not find the error file;
				// we have to send the response as is, with no content (that is, if set_body() hasn't been called earlier)
			}
		}
		
		/**
		 * write response to client and exit thread
		 */
		client->send(response);
		client->close();
		
		// delete [] buf;
		delete request;
		delete response;
		delete client;
	}
}

void webserver::handle_request (http::request* request, http::response* response)
{
	/**
	 * Try to load the following files, in prioritized order:
	 * 1. a module file
	 * 2. Directory indexes
	 * 3. The requested file
	 */
	
	std::string* filename = new std::string (config::HTML_ROOT);
	filename->append (request->file());
	
	std::string* module = new std::string(config::MODULES_ROOT);
	module->append (request->file());
	module->append(config::MODULE_EXT);
	
	if ( ! utils::fileutils::is_file (*filename) && ! utils::fileutils::is_directory (*filename) && ! utils::fileutils::is_file (*module))
	{
		response->set_status(404);
		response->set_body ("Could not find the requested path: " + *filename);
		
		delete module;
		delete filename;
		return;
	}
	
	if ( utils::fileutils::is_file (*module))
	{
		if (! load_module (request, response, *module))
		{
			response->set_status(500);
			response->set_body ("Could not load or call the requested module: " + *module);
		}
		
		delete module;
		delete filename;
		return;
	}
	
	delete module;
	
	if ( utils::fileutils::is_directory (*filename))
	{
		/**
		 * Send a 301 Header if request points to a directory, but the URI doesn't ends with a forward slash, /.
		 */
		if ( *filename->rbegin() != '/' )
		{
			response->set_status(302);
			response->set_header ("Location", request->url() + request->uri() + "/");
			response->set_body ("Object has moved");
		}
		else if ( ! load_file (response, filename->append("index.html")) 
			   && ! load_file (response, filename->append("index.htm")))
		{
			// if we reach this position, an index file could not be loaded although the directory exists;
			// this means we have to respond with a 301 status.
			response->set_status(301);
			response->set_body ("Access Forbidden!");
		}
		
		delete filename;
		return;
	}
	
	if (! load_file (response, *filename))
	{
		response->set_status(500);
		response->set_body ("Could not load the file: " + *filename);
	}
		
	delete filename;
	return;
}

bool webserver::load_module (http::request* request, http::response* response, std::string filename)
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
		return false;
	}
	
	return true;
}

bool webserver::load_file (http::response* response, std::string filename)
{
	if ( ! utils::fileutils::is_file (filename) )
	{
		return false;
	}
	
	try
	{
		// load contents of file
		response->set_body (utils::fileutils::contents(filename));
		
		/**
		 * Find mime type based on file extension,
		 * and default to text/plain if no mime can be found
		 */
		unsigned ext_pos = filename.find_last_of(".");
		if (ext_pos != std::string::npos && ext_pos <= filename.length())
			response->set_content_type (utils::fileutils::content_type (filename.substr(ext_pos), "text/plain"));
		
		return true;
	}
	catch (int e)
	{
		return false;
	}
}

#endif
