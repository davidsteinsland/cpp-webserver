#include "config.h"
#include "webserver/webserver.h"

#include <iostream>
#include <csignal>

webserver::webserver* ws;
static int exit_flag;

static void WINCDECL signal_handler(int sig_num)
{
	exit_flag = sig_num;
	
	ws->shutdown();
	delete ws;
	
	std::cout << "Shutting down ..." << std::endl;
}

int main()
{
	/* Setup signal handler: quit on Ctrl-C */
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
  
	ws = new webserver::webserver(config::PORT);
	int error;
	if ( (error = ws->listen()) != 0)
	{
		std::cout << "Error: " << std::endl;
		std::cout << strerror (error) << std::endl;
	}

	return 0;
}
