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
#include "http/response.h"

#include <string>

#define DEFAULT_BUFFER_SIZE 512

namespace net
{
	class clientsocket
	{
		private:
			SOCKET socket;
			struct sockaddr_in address;
			
		public:
			clientsocket(SOCKET s,struct sockaddr_in addr) : socket(s), address(addr)
			{
			
			}
			
			~clientsocket();
			
			void close();
			bool valid();
			int recieve(char*,int);
			int send(http::response*);
			int send (std::string);
			int send(const char*,int,int);
	};
};

#endif