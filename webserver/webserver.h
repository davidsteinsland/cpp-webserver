/**
 * webserver.h
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H 

#include "../http/socket.h"
#include "../http/response.h"
#include "../http/request.h"
 
namespace webserver
{
	class webserver
	{
		private:
			http::socket* listenSocket;
			int port;
		
		public:
			webserver(int);
			~webserver();
			
			int listen();
	};
};

#endif