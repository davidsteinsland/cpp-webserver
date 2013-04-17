#include <iostream>
#include "webserver/webserver.h"
#include "config.h"

#ifdef _WIN32
	#include <string.h> /* _strerror() */
	#include <malloc.h> /* _alloca() */
	#define strerror(x)  _ultoa(x, (char *) _alloca(sizeof(x) *3 ), 10)
	#define WINCDECL __cdecl
#else
	#define WINCDECL
	#include <cstring> /* strerror() */
#endif

#include <signal.h>

webserver::webserver* ws;
int exit_flag;

static void WINCDECL signal_handler(int sig_num)
{
	exit_flag = sig_num;
	
	ws->shutdown();
	delete ws;
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
	
	std::cout << "Shutting down ..." << std::endl;
}
