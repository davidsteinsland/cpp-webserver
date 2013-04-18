/**
 * clientsocket.h
 */
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H 

#ifdef _WIN32
	#include <winsock2.h>
	#define ERRNO GetLastError()
#else
	#define ERRNO errno
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/types.h>
#endif

#include "http/response.h"

#include <string>
#include <iostream>
#include <string>
#include <map>

#define DEFAULT_BUFFER_SIZE 512

namespace net
{
	class clientsocket
	{
		private:
			int socket;
			struct sockaddr_in address;
			
		public:
			clientsocket(int s,struct sockaddr_in addr) : socket(s), address(addr)
			{
			
			}
			
			~clientsocket()
			{
				close();
			}
			
			bool valid();
			void close();
			
			std::string recieve()
			{
				char requestBuffer[DEFAULT_BUFFER_SIZE];

				int k = ::recv(socket, requestBuffer, DEFAULT_BUFFER_SIZE, 0);
				
				if ( k == 0 )
				{
					std::cout << "Connection closed" << std::endl;
				}
				else if (k == SOCKET_ERROR)
				{
					std::cout << "recv failed: " << ERRNO << std::endl;
					close();
				}
				
				return std::string(requestBuffer, 0, k);
			}
			
			int send(http::response* res)
			{
				std::string headersString = res->status_line();
				std::map<std::string,std::string> headers = res->headers();
				
				for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
					headersString.append (it->first + ": " + it->second + "\r\n");
				headersString.append("\r\n");
				
				return send (headersString + res->body());
			}
			
			int send (std::string data)
			{
				return send (data.c_str(), data.length(), 0);
			}
			
			int send(const char* buf,int len,int flags)
			{
				return ::send (socket, buf, len, flags);
			}
	};
};

#endif
