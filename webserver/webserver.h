/**
 * webserver.h
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H 

#include "net/serversocket.h"
#include "net/clientsocket.h"
#include "webserver/worker_pool.h"
 
namespace webserver
{
	class webserver
	{
		private:
			worker_pool<net::clientsocket*>* thread_pool;
			net::serversocket* listenSocket;
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