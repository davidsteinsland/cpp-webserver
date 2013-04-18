#ifndef SOCKET_CPP
#define SOCKET_CPP 

#include "net/socket.h"
#include <unistd.h>

void net::socket::close()
{
	listening = false;
	::close(socket);
}

#endif
