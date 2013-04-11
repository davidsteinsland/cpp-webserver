#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP 

#include "webserver.h"
#include "../net/socket.h"
#include "../http/response.h"
#include "../http/request.h"
#include "../utils/ioutils.h"

#ifdef TARGET_OS_MAC
	#error Mac is not supported yet
#endif

#ifdef __linux__
	#include "../net/unix/socket.cpp"
#endif

#ifdef _WIN32
	#include "../net/win32/socket.cpp"
	#include "../net/win32/clientsocket.cpp"
#endif

#include <string>
#include <iostream>

webserver::webserver::webserver(int p)
{
	port = p;
	listenSocket = new net::socket();
	
	std::cout << "WebServer initiated" << std::endl;
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
		// std::cout << "Error creating socket" << std::endl;
		return 0;
	}
	
	// std::cout << "WebServer listening..." << std::endl;
	
	while (true)
	{
		net::clientsocket* client = listenSocket->get_connection();
		
		if ( client == NULL )
			continue;
		
		http::request* request = client->get_request();
		
		std::cout << "Got a connection!" << std::endl;
		std::cout << "URL: " << request->get_url() << std::endl;
		std::cout << "URI: " << request->get_uri() << std::endl;
		std::cout << "QS: " << request->get_query_string() << std::endl;
		std::cout << "Host: " << request->get_header("Host") << std::endl;
		
		std::string html_dir = "./public_html";
		std::string filename = html_dir + request->get_uri();
		// std::string file = utils::ioutils::get_file_contents ("public_html/index.html");
		
		delete request;
		
		/**
		 * Try to load the following files, in prioritized order:
		 * 1. The requested file
		 * 2. Directory index
		 * 3. 404 page
		 */
		std::string filenames[3] = {filename, filename + "index.html", html_dir + "/404.html"};
		std::string contents;
		bool file_found = false;
		for (int i = 0; i < 3; i++)
		{
			try
			{
				filename = filenames[i];
				contents = utils::ioutils::get_file_contents(filename);
				file_found = true;
				break;
			}
			catch (...)
			{ }
		}
		
		std::string content_type = utils::ioutils::get_mime_type (filename.substr(filename.find_last_of(".")));	
		int status_code = 200;
		
		if ( ! file_found || filename == filenames[2])
			status_code = 404;
		if ( ! file_found )
		{
			content_type = "";
		}
		
		
		http::response* response = new http::response (status_code, content_type);
		response->set_body(contents);
		
		/**
		 * write response to client
		 */
		std::string headersString = response->response_line();
		std::map<std::string,std::string> headers = response->headers();
		
		for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
			headersString.append (it->first + ": " + it->second + "\r\n");
		headersString.append("\r\n");
		
		client->send (headersString.c_str(), (int)headersString.length(), 0);
		client->send (response->body().c_str(), (int)response->body().length(), 0);
		
		client->close();
		delete client;
		delete response;
	}
	
	return 1;
}

#endif