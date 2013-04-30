#ifndef SERVER_SOCKET_CPP
#define SERVER_SOCKET_CPP 

#include "net/serversocket.h"
#include "net/socket_types.h"
#include "net/clientsocket.h"

#include <cstring> /* memset */

#define BACKLOG 20

net::serversocket::~serversocket()
{
	close();
}

int net::serversocket::listen(int port)
{
	if (is_active)
		return -1;

	#ifdef _WIN32
	WSAData wsaData;

	if (WSAStartup (0x0202, &wsaData) != NO_ERROR)
	{
		return ERRNO; //For some reason we couldn't start Winsock
	}

	if (wsaData.wVersion != 0x0202) //Wrong Winsock version?
	{
		WSACleanup ();
		return ERRNO;
	}
	#endif
	
	memset (&address, 0, sizeof (address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	//Accept a connection from any IP using INADDR_ANY
	//You could pass inet_addr("0.0.0.0") instead to accomplish the 
	//same thing. If you want only to watch for a connection from a 
	//specific IP, specify that instead.
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	
	socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if ( socket == INVALID_SOCKET)
	{
		return ERRNO;
	}

	#ifdef __linux
	int yes = 1;
	if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0)
	{
		close();
		return ERRNO;
	}
	#endif

	if (::bind (socket, (struct sockaddr*)&address, sizeof(struct sockaddr)) != 0)
	{
		close();
		return ERRNO;
	}

	if (::listen (socket, BACKLOG) != 0)
	{
		close();
		return ERRNO;
	}
	
	is_active = true;

	return 0;
}

net::clientsocket* net::serversocket::accept()
{
	struct sockaddr_in from;
	socklen_t l = sizeof (from);
	SOCKET clientfd = ::accept (socket, (struct sockaddr*)&from, &l);
	
	clientsocket* clientSocket = new clientsocket(clientfd, from);
	
	if (clientfd == INVALID_SOCKET )
	{
		delete clientSocket;
		return NULL;
	}
	
	return clientSocket;
}

#endif
