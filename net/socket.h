/**
 * socket.h
 */
#ifndef SOCKET_H
#define SOCKET_H

#include "clientsocket.h"

namespace http
{
	class socket
	{
		public:
			int listen(int);
			clientsocket* get_connection();
			void close();
	};
};

#endif