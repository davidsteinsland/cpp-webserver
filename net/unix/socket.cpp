#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include "../socket.h"
#include "../clientsocket.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

net::socket::~socket()
{
	close();
}

int net::socket::listen(int port)
{
	if (listening)
		return 0;
	
	if ( (socket = ::socket(PF_INET, SOCK_STREAM, 0)) == -1)
		return 0;

	int yes = 1;
	if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close();
		return 0;
	}
	
	address.sin_family = AF_INET;
	address.sin_port = port;
	address.sin_addr.s_addr = INADDR_ANY;
	memset(&(address.sin_zero), '\0', 8);

	if (::bind (socket, (struct sockaddr*)&address, sizeof(struct sockaddr)) == -1)
	{
		close();
		return 0;
	}

	if (::listen (socket, 5) == -1)
	{
		close();
		return 0;
	}

	return 1;
}

void net::socket::close()
{
	listening = false;
	::close(sockfd);
}

net::clientsocket* net::socket::accept()
{
	struct sockaddr_in client;
	int l = sizeof (client);
	int msgSocket = ::accept (sockfd, (struct sockaddr*)&client, (socklen_t*)&l);

	net::clientsocket* clientsocket = new net::clientsocket (msgSocket, client);

	if (msgSocket == -1)
	{
		delete clientsocket;
		return NULL;
	}

	return clientsocket;
}

#endif
