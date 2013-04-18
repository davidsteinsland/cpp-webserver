#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include "net/clientsocket.h"
#include <unistd.h>

bool net::clientsocket::valid()
{
	return socket != -1;
}

void net::clientsocket::close()
{
	::close(socket);
}

#endif