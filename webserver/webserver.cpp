#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP 


#include "config.h"
#include "webserver/webserver.h"
#include "webserver/worker.h"
#include "net/socket.h"
#include "net/clientsocket.h"

#include <string>
#include <iostream>

#include <unistd.h>

webserver::webserver::webserver(int p)
{
	port = p;
	listenSocket = new net::socket();
	
	thread_pool = new worker_pool<net::clientsocket*> (config::NUM_WORKER_THREADS);
}

webserver::webserver::~webserver()
{
	listenSocket->close();
	
	delete thread_pool;
	delete listenSocket;
}

void webserver::webserver::shutdown()
{
	thread_pool->shutdown();
	started = false;
}

int webserver::webserver::listen ()
{
	int error;
	if ( (error = listenSocket->listen(port)) != 0)
	{
		return error;
	}
	
	started = true;
		
	// starting worker threads
	thread_pool->start_workers(worker_thread);
	
	while (listenSocket->active())
	{
		net::clientsocket* client = listenSocket->accept();
		
		if ( client == NULL || ! client->valid() )
		{
			delete client;
			continue;
		}
		
		thread_pool->add_job (client);
	}
	
	return 0;
}

#endif