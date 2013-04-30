/**
 * clientsocket.h
 */
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H 

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

#include "net/socket_types.h"
#include "net/socket.h"
#include "http/response.h"

#include <string>

namespace net
{
	class clientsocket : public socket
	{
		public:
			clientsocket(SOCKET s,struct sockaddr_in addr)
			{
				socket = s;
				address = addr;
				is_active = true;
			}
			
			~clientsocket();
			
			int read_fully (std::string&);
			int recieve(char*,int);
			int send(http::response*);
			int send (std::string);
			int send(const char*,int,int);
	};
};

#endif