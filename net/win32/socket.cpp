#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include <winsock.h>
#include "net/socket.h"

void net::socket::close ()
{
	listening = false;
	closesocket(socket);
    WSACleanup();
}

#endif
