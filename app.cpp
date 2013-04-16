#include <iostream>
#include "webserver/webserver.h"
#include "config.h"

#ifdef _WIN32
#include <string.h> /* _strerror() */
#else
#include <cstring> /* strerror() */
#endif

using namespace std;

int main()
{
	webserver::webserver* ws = new webserver::webserver(config::PORT);
	int error;
	if ( (error = ws->listen()) != 0)
	{
		cout << "Error: " << endl;
		#ifdef _WIN32
		cout << _strerror(error) << endl;
		#else
		cout << strerror (error) << endl;
		#endif
	}
	
	delete ws;
}
