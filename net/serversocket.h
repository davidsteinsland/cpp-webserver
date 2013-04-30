/**
 * socket.h
 */
#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <sys/types.h>
#endif

#include "net/socket_types.h"
#include "net/serversocket.h"
#include "net/clientsocket.h"

namespace net
{
	class serversocket : public socket
	{
		public:
			~serversocket();
			int listen(int);
			clientsocket* accept();
	};
};

#endif