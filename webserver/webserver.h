/**
 * webserver.h
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H 

#include "net/socket.h"
#include "http/response.h"
#include "http/request.h"
 
namespace webserver
{
	class webserver
	{
		private:
			net::socket* listenSocket;
			int port;
		
		public:
			webserver(int);
			~webserver();
			
			int listen();
	};
};

#endif