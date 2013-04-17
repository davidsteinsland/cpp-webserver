#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include "net/socket.h"
#include "net/clientsocket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring> /* memset */
#include <cerrno>

net::socket::~socket()
{
	close();
}

int net::socket::listen(int port)
{
	if (listening)
		return -1;

	memset (&address, 0, sizeof (address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	//memset(&(address.sin_zero), '\0', 8);

	if ( (socket = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return errno;
	}

	int yes = 1;
	if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close();
		return errno;
	}

	if (::bind (socket, (struct sockaddr*)&address, sizeof(struct sockaddr)) == -1)
	{
		close();
		return errno;
	}

	if (::listen (socket, 5) == -1)
	{
		close();
		return errno;
	}
	
	listening = true;

	return 0;
}

void net::socket::close()
{
	listening = false;
	::close(socket);
}

net::clientsocket* net::socket::accept()
{
	struct sockaddr_in client;
	int l = sizeof (client);
	int msgSocket = ::accept (socket, (struct sockaddr*)&client, (socklen_t*)&l);

	net::clientsocket* clientsocket = new net::clientsocket (msgSocket, client);

	if (msgSocket == -1)
	{
		delete clientsocket;
		return NULL;
	}

	return clientsocket;
}

#endif
