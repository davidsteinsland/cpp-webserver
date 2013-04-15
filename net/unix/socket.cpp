#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include "../socket.h"
#include "../clientsocket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

int sockfd;
int net::socket::listen(int port)
{
	if ( (sockfd = ::socket(PF_INET, SOCK_STREAM, 0)) == -1)
		return 0;

	int yes = 1;
	if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close();
		return 0;
	}
	
	struct sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_port = port;
	serverService.sin_addr.s_addr = INADDR_ANY;
	memset(&(serverService.sin_zero), '\0', 8);

	if (::bind (sockfd, (struct sockaddr*)&serverService, sizeof(struct sockaddr)) == -1)
	{
		close();
		return 0;
	}

	if (::listen (sockfd, 5) == -1)
	{
		close();
		return 0;
	}
}

void net::socket::close()
{
	::close(sockfd);
}

net::clientsocket* net::socket::get_connection()
{
	struct sockaddr_in client;
	int l = sizeof (client);
	int msgSocket = ::accept (sockfd, (struct sockaddr*)&client, &l);

	net::clientsocket* clientSocket = new net::clientsocket (msgSocket, from);

	if (msgSocket == -1)
	{
		clientsocket->close();
		delete clientsocket;
		return NULL;
	}

	return clientSocket;
}

#endif
