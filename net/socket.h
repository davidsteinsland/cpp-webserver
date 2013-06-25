/**
 * an interface for both listen socket and client socket
 */

#ifndef SOCKET_H
#define SOCKET_H

#include "net/socket_types.h"

namespace net
{
	class socket
	{
		protected:
			SOCKET socket;
			struct sockaddr_in address;
			bool is_active;
		
		public:
			void close()
			{
				is_active = false;
				
				if (socket != INVALID_SOCKET)
				{
					::closesocket(socket);
				}
			}
			
			bool active()
			{
				return is_active;
			}
			
			bool valid()
			{
				return socket != INVALID_SOCKET;
			}
			
			SOCKET get_socket()
			{
				return socket;
			}
	};
};

#endif
