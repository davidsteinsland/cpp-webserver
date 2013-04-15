/**
 * clientsocket.h
 */
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H 

#include "../http/request.h"

#ifdef __linux__
typedef struct SOCKADDR_IN client;
#endif

namespace net
{
	class clientsocket
	{
		private:
		#ifdef _WIN32
			SOCKET socket;
			u_short client_port;
		#else
			int socket;
			int client_port;
		#endif
			char* client_address;
			std::string body;
			int bytesRecieved;
			
		public:
			#ifdef _WIN32
			clientsocket(SOCKET,SOCKADDR_IN);
			#else
			clientsocket(int,SOCKADDR_IN);
			#endif
			~clientsocket();
			
			http::request* get_request();
			int send(const char*,int,int);
			void close();
	};
};

#endif
