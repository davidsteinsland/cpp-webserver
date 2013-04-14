#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include "../clientsocket.h"
#include "../../http/request.h"
#include <iostream>

#define DEFAULT_BUFFER_SIZE 512

net::clientsocket::clientsocket (SOCKET s, SOCKADDR_IN client)
{
	socket = s;
	
	client_address = inet_ntoa (client.sin_addr);
	client_port = ntohs(client.sin_port);
	
	char requestBuffer[DEFAULT_BUFFER_SIZE];

	bytesRecieved = recv(socket, requestBuffer, DEFAULT_BUFFER_SIZE, 0);
	
	if ( bytesRecieved == 0 )
	{
		std::cout << "Connection closed" << std::endl;
	}
	else if (bytesRecieved == SOCKET_ERROR)
	{
		std::cout << "recv failed: " << WSAGetLastError() << std::endl;
		this->close();
		WSACleanup();
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
	return ::send (socket, buf, len, flags);
}

void net::clientsocket::close()
{
	closesocket(socket);
}

#endif