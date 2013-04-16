#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include <winsock.h>
#include <iostream>
#include <stdio.h>
#include <map>
#include "../socket.h"
#include "../clientsocket.h"
#include "../../http/request.h"

net::socket::~socket()
{
	close();
}

int net::socket::listen(int port)
{
	if (listening)
		return 0;

	WSAData wsaData;
	
	if (WSAStartup (0x0202, &wsaData) != NO_ERROR)
	{
		return 0; //For some reason we couldn't start Winsock
    }

    if (wsaData.wVersion != 0x0202) //Wrong Winsock version?
    {
        WSACleanup ();
        return 0;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons (port);

    //Accept a connection from any IP using INADDR_ANY
    //You could pass inet_addr("0.0.0.0") instead to accomplish the 
    //same thing. If you want only to watch for a connection from a 
    //specific IP, specify that instead.
    address.sin_addr.s_addr = htonl (INADDR_ANY);  

    socket = (int) ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if ((SOCKET)socket == INVALID_SOCKET)
    {
        return 0; 
    }

	if (bind ((SOCKET)socket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
	{
		close();
		return 0;
	}

	if(::listen(socket, 5) == SOCKET_ERROR)
	{
		close();
		return 0;
	}
	
	listening = true;
	
	return 1;
}

net::clientsocket* net::socket::accept ()
{
	SOCKADDR_IN from;
	int l = sizeof (from);
	int clientfd = ::accept (socket, (struct sockaddr*)&from, &l);
	
	clientsocket* clientSocket = new clientsocket(clientfd, from);
	
	if ((SOCKET)clientfd == INVALID_SOCKET)
	{
		delete clientSocket;
		return NULL;
	}
	
	return clientSocket;
}

void net::socket::close ()
{
	listening = false;
	closesocket(socket);
    WSACleanup();
}

#endif