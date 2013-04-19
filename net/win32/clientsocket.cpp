#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP 

#include <winsock2.h>
#include "net/clientsocket.h"

bool net::clientsocket::valid()
{
	return (SOCKET)socket != INVALID_SOCKET;
}

void net::clientsocket::close()
{
	if ((SOCKET)socket != INVALID_SOCKET)
		closesocket(socket);
}

#endif