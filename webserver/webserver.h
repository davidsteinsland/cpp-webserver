/**
 * webserver.h
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H 

#include "net/socket.h"
#include "net/clientsocket.h"
#include "http/response.h"
#include "http/request.h"
 
namespace webserver
{
	void* worker_thread(void*);
	
	void handle_request(net::clientsocket*);
	bool load_module (http::request*,http::response*);
	bool load_file (http::request*, http::response*);
	
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