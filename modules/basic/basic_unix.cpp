/**
 * Dynamic Page module for Windows
 */
 
#include "../../http/request.h"
#include "../../http/response.h"
#include <string>
#include <sstream>

extern "C" int handle_request(http::request* request, http::response* response)
{
	response->set_status(200);
	response->set_content_type ("text/html");
	
	std::stringstream ss;
	ss << "<h1>This is my first Dynamic Page!</h1>";
	ss << "<p>1 + 1 = " << 2 << "</p>";
	
	response->set_body(ss.str());
	
	return 1;
}