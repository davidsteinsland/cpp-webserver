#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include <winsock.h>
#include <iostream>
#include <stdio.h>
#include <map>
#include "../socket.h"
#include "../clientsocket.h"
#include "../../http/request.h"

SOCKET serverSocket;
WSAData wsaData;

int net::socket::listen(int port)
{
	if (WSAStartup (0x0202, &wsaData) != NO_ERROR)
	{
		return 0; //For some reason we couldn't start Winsock
    }

    if (wsaData.wVersion != 0x0202) //Wrong Winsock version?
    {
        WSACleanup ();
        return 0;
    }

    SOCKADDR_IN serverService;
    serverService.sin_family = AF_INET;
    serverService.sin_port = htons (port);

    //Accept a connection from any IP using INADDR_ANY
    //You could pass inet_addr("0.0.0.0") instead to accomplish the 
    //same thing. If you want only to watch for a connection from a 
    //specific IP, specify that instead.
    serverService.sin_addr.s_addr = htonl (INADDR_ANY);  

    serverSocket = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        return 0; 
    }

	if(bind(serverSocket, (SOCKADDR*)&serverService, sizeof(serverService)) == SOCKET_ERROR){
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}

	if(::listen(serverSocket, 5) == SOCKET_ERROR){
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	
	return 1;
}

net::clientsocket* net::socket::get_connection ()
{
	SOCKADDR_IN from;
	int l = sizeof (from);
	SOCKET msgSocket = accept (serverSocket, (SOCKADDR*)&from, &l);
	
	clientsocket* clientSocket = new clientsocket(msgSocket, from);
	
	if (msgSocket == INVALID_SOCKET)
	{
		clientSocket->close();
		delete clientSocket;
		return NULL;
	}
	
	return clientSocket;
}

void net::socket::close ()
{
	 //Close the socket if it exists
    if (serverSocket)
        closesocket(serverSocket);

    WSACleanup(); //Clean up Winsock
}

#endif