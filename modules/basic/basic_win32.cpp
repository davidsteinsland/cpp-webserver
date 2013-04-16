/**
 * Dynamic Page module for Windows
 */
 
#include "../../http/request.h"
#include "../../http/response.h"
#include <windows.h>
#include <string>
#include <sstream>

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return 0;
}

extern "C" __declspec (dllexport) int handle_request(http::request* request, http::response* response)
{
	response->set_status(200);
	response->set_content_type ("text/html");
	
	std::stringstream ss;
	ss << "<h1>This is my first Dynamic Page!</h1>";
	ss << "<p>1 + 1 = " << 2 << "</p>";
	
	response->set_body(ss.str());
	
	return 1;
}