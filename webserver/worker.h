/**
 * worker class
 * handles the incoming requests
 */
#ifndef WORKER_H
#define WORKER_H

#include "http/response.h"
#include "http/request.h"
#include <string>

namespace webserver
{
	void* worker_thread(void*);
	
	void handle_request(http::request*,http::response*);
	bool load_module (http::request*,http::response*, std::string);
	bool load_file (http::response*,std::string);
};

#endif