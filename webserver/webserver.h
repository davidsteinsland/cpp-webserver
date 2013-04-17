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
	void* handle_request(void*);
	
	class webserver
	{
		private:
			net::socket* listenSocket;
			int port;
			bool started;
			
		public:
			webserver(int);
			~webserver();
			void shutdown();
			
			int listen();
	};
};

#endif