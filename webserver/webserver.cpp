#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP 

#include "webserver.h"
#include "../http/response.h"
#include "../http/request.h"
#include "../http/socket.h"
#include "../utils/ioutils.h"

#ifdef TARGET_OS_MAC
	#error Mac is not supported yet
#endif

#ifdef __linux__
	#include "../http/unix/socket.cpp"
#endif

#ifdef _WIN32
	#include "../http/win32/socket.cpp"
	#include "../http/win32/clientsocket.cpp"
#endif

#include <string>
#include <iostream>

webserver::webserver::webserver(int p)
{
	port = p;
	listenSocket = new http::socket();
	
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
		std::cout << "Error creating socket" << std::endl;
		return 0;
	}
	
	std::cout << "WebServer listening..." << std::endl;
	
	while (true)
	{
		http::clientsocket* client = listenSocket->get_connection();
		
		if ( client == NULL )
			continue;
		
		http::request* request = client->get_request();
		
		std::cout << "Got a connection!" << std::endl;
		// std::cout << request->body() << std::endl;
		std::cout << "URL: " << request->get_url() << std::endl;
		std::cout << "URI: " << request->get_uri() << std::endl;
		std::cout << "QS: " << request->get_query_string() << std::endl;
		std::cout << "Host: " << request->get_header("Host") << std::endl;
		
		std::string html_dir = "./public_html";
		std::string filename = html_dir + request->get_uri();
		// std::string file = utils::ioutils::get_file_contents ("public_html/index.html");
		
		std::string contents;
		
		/**
		 * Try to load the following files, in priority:
		 * 1. The requested file
		 * 2. Directory index
		 * 3. 404 page
		 */
		std::string filenames[3] = {filename, filename + "index.html", html_dir + "/404.html"};
		bool file_found = false;
		for (int i = 0; i < 3; i++)
		{
			try
			{
				contents = utils::ioutils::get_file_contents(filenames[i]);
				file_found = true;
				break;
			}
			catch (...)
			{ }
		}
		
		std::string headers;		
	
		if ( ! file_found )
		{
			headers = "HTTP/1.1 404 Not Found";
			contents = "";
		}
		else
		{
			headers = "HTTP/1.1 200 OK \r\n";
			headers.append ("Content-Type: text/html \r\n");
		}
		
		char* htmp = new char[100];
		sprintf (htmp, "Content-Length: %ld \r\n\r\n", (int)contents.length());
		headers.append(htmp);
		delete [] htmp;
		
		client->send (headers.c_str(), (int)headers.length(), 0);
		client->send (contents.c_str(), (int)contents.length(), 0);
		
		client->close();
		delete client;
	}
	
	return 1;
}

#endif