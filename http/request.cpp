#ifndef REQUEST_CPP
#define REQUEST_CPP

#include "request.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>

std::stringstream& readline (std::stringstream &ss, std::string &line)
{
	std::getline(ss, line, '\n');
	
	if (line[line.size() - 1] == '\r')
		line.resize(line.size() - 1);
	
	return ss;
}

http::request::request (std::string requestBody)
{
	reqBody = requestBody;
	this->parse (reqBody);
}

/**
 * Request       = Request-Line              ; Section 5.1
				*(( general-header        ; Section 4.5
				 | request-header         ; Section 5.3
				 | entity-header ) CRLF)  ; Section 7.1
				CRLF
				[ message-body ]          ; Section 4.3
*/
void http::request::parse (std::string str)
{
	std::stringstream ss(str);
	std::string line;
	int line_num = 0;
	
	while (readline(ss, line) && line.size() > 0)
	{
		++line_num;
		std::stringstream lineStream(line);
		
		if (line_num == 1)
		{
			std::getline (lineStream, type, ' ');
			std::getline (lineStream, uri, ' ');
			
			if ( uri.find('?') != std::string::npos)
				queryString = uri.substr (uri.find('?'));
			
			readline (lineStream, protocol);
		}
		else
		{
			std::string key, value;
			std::getline (lineStream, key, ':');
			std::getline (lineStream, value);
			
			if ( value[0] == ' ')
				value.erase (0, 1);
			
			headers.insert (std::pair<std::string,std::string>(key, value));
		}
		
		url = std::string("http://");
		url.append (headers.find("Host")->second);
	}
}

#endif