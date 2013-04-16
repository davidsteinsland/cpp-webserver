#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include "../clientsocket.h"
#include "../../http/response.h"
#include <map>
#include <string>

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define DEFAULT_BUFFER_SIZE 512

net::clientsocket::clientsocket (int s, struct sockaddr_in addr) : socket(s), address(addr)
{
	
}

net::clientsocket::~clientsocket()
{
	close();
}

std::string net::clientsocket::recieve()
{
	char requestBuffer[DEFAULT_BUFFER_SIZE];

	bytesRecieved = read(socket, requestBuffer, DEFAULT_BUFFER_SIZE);
	
	if ( bytesRecieved == 0 )
	{
		std::cout << "Connection closed" << std::endl;
	}
	else if (bytesRecieved == -1)
	{
		std::cout << "recv failed: " << std::endl;
		this->close();
	}
	
	return std::string(requestBuffer, 0, bytesRecieved);
}

int net::clientsocket::send (http::response* res)
{
	std::string headersString = res->status_line();
	std::map<std::string,std::string> headers = res->headers();
	
	for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		headersString.append (it->first + ": " + it->second + "\r\n");
	headersString.append("\r\n");
	
	return send (headersString + res->body());
}

int net::clientsocket::send (std::string data)
{
	return send (data.c_str(), data.length(), 0);
}

int net::clientsocket::send(const char* buf, int len, int flags)
{
	return ::send (socket, buf, len, 0);
}

void net::clientsocket::close()
{
	::close(socket);
}

#endif
