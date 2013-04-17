/**
 * socket.h
 */
#ifndef SOCKET_H
#define SOCKET_H

#include "net/clientsocket.h"

namespace net
{
	class socket
	{
		private:
			bool listening;
			int socket;
			struct sockaddr_in address;
		public:
			~socket();
			int listen(int);
			clientsocket* accept();
			void close();
	};
};

#endif