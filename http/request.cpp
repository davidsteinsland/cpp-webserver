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

http::request::request (std::string body)
{
	request_body = body;
	this->parse (body);
}

http::request::~request()
{

}

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
			std::getline (lineStream, request_uri, ' ');
			
			if ( request_uri.find('?') != std::string::npos)
			{
				request_query_string = request_uri.substr (request_uri.find('?'));
				
				std::stringstream qss(request_query_string.substr(1));
				
				std::string param;
				while ( std::getline(qss, param, '&') )
				{
					std::string key, val;
					
					key = param.substr(0, param.find('='));
					val = param.substr (param.find('=') + 1);
					
					query_string_map.insert(std::pair<std::string,std::string>(key,val));
				}
			}
			
			readline (lineStream, protocol);
		}
		else
		{
			std::string key, value;
			std::getline (lineStream, key, ':');
			std::getline (lineStream, value);
			
			if ( value[0] == ' ')
				value.erase (0, 1);
			
			this->header(key, value);
		}
		
		request_url = std::string("http://");
		request_url.append (this->header("Host"));
	}
}

void http::request::header(std::string k,std::string v)
{
	headers.insert (std::pair<std::string,std::string>(k, v));
}

#endif