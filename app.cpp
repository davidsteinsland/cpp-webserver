#include <iostream>
#include "webserver/webserver.h"

#define PORT 80

using namespace std;

int main()
{
	webserver::webserver* ws = new webserver::webserver(PORT);
	
	if (ws->listen() == 0)
		cout << "Error!" << endl;
	
	delete ws;
}