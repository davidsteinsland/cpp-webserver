#include <iostream>
#include "webserver/webserver.h"
#include "config.h"

using namespace std;

int main()
{
	webserver::webserver* ws = new webserver::webserver(config::PORT);
	
	if (ws->listen() == 0)
		cout << "Error!" << endl;
	
	delete ws;
}