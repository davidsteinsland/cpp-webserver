#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include <winsock.h>
#include "../clientsocket.h"
#include "../request.h"
#include <iostream>

#define DEFAULT_BUFFER_SIZE 512

http::clientsocket::clientsocket (SOCKET s, SOCKADDR_IN client)
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

http::clientsocket::~clientsocket()
{
	
}

http::request* http::clientsocket::get_request()
{
	return new http::request(body);
}

int http::clientsocket::send(const char* buf, int len, int flags)
{
	return ::send (socket, buf, len, flags);
}

void http::clientsocket::close()
{
	closesocket(socket);
}

#endif