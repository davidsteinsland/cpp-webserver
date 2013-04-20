/**
 * socket.h
 */
#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <sys/types.h>
#endif

#include "net/socket_types.h"
#include "net/clientsocket.h"

namespace net
{
	class socket
	{
		private:
			bool listening;
			SOCKET socket;
			struct sockaddr_in address;
		public:
			~socket();
			void close();
			bool active();
			int listen(int);
			clientsocket* accept();
	};
};

#endif