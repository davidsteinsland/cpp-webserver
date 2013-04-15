#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include "../clientsocket.h"
#include "../../http/request.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define DEFAULT_BUFFER_SIZE 512

net::clientsocket::clientsocket (int s, SOCKADDR_IN client)
{
	socket = s;
	
	client_address = inet_ntoa (client.sin_addr);
	client_port = ntohs(client.sin_port);
	
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
	
	body = std::string(requestBuffer, 0, bytesRecieved);
}

net::clientsocket::~clientsocket()
{
	delete client_address;
}

http::request* net::clientsocket::get_request()
{
	return new http::request(body);
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
