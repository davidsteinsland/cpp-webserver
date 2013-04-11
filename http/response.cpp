#ifndef RESPONSE_CPP
#define RESPONSE_CPP

#include <string>
#include <map>
#include "response.h"

std::map<std::string,std::string> http::response::status_codes(http::response::create_status_lines());

#endif