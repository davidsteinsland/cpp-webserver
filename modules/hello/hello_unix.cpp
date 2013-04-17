/**
 * Dynamic Page module for Windows
 */
 
#include "http/request.h"
#include "http/response.h"
#include "utils/stringutils.h"

#include <string>
#include <sstream>

extern "C" int handle_request(http::request* request, http::response* response)
{
	response->set_status(200);
	response->set_content_type ("text/html");
	
	std::string person = request->query_string("name", "Mr. No Name!");
	person = utils::stringutils::urldecode (person);
	
	std::stringstream ss;
	ss << "<h1>Hello, " << person << "!</h1>";
	
	response->set_body(ss.str());
	
	return 1;
}
