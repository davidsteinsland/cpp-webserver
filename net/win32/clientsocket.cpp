#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include <winsock2.h>
#include "net/clientsocket.h"
#include "http/response.h"
#include <iostream>
#include <string>
#include <map>

#define DEFAULT_BUFFER_SIZE 512

net::clientsocket::clientsocket (int s, struct sockaddr_in addr) : socket(s), address(addr)
{
	
}

net::clientsocket::~clientsocket()
{
	close();
}

bool net::clientsocket::valid()
{
	return (SOCKET)socket != INVALID_SOCKET;
}

std::string net::clientsocket::recieve()
{
	char requestBuffer[DEFAULT_BUFFER_SIZE];

	int k = recv(socket, requestBuffer, DEFAULT_BUFFER_SIZE, 0);
	
	if ( k == 0 )
	{
		std::cout << "Connection closed" << std::endl;
	}
	else if (k == SOCKET_ERROR)
	{
		std::cout << "recv failed: " << WSAGetLastError() << std::endl;
		close();
		WSACleanup();
	}
	
	return std::string(requestBuffer, 0, k);
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

int net::clientsocket::send(std::string data)
{
	return send (data.c_str(), data.length(), 0);
}

int net::clientsocket::send(const char* buf, int len, int flags)
{
	return ::send (socket, buf, len, flags);
}

void net::clientsocket::close()
{
	closesocket(socket);
}

#endif