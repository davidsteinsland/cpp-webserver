/**
 * clientsocket.h
 */
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H 
 
#include <winsock.h>
#include "request.h"

namespace http
{
	class clientsocket
	{
		private:
			SOCKET socket;
			char* client_address;
			u_short client_port;
			std::string body;
			int bytesRecieved;
			
		public:
			clientsocket(SOCKET,SOCKADDR_IN);
			~clientsocket();
			
			http::request* get_request();
			int send(const char*,int,int);
			void close();
	};
};

#endif