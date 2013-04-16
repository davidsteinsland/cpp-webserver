#ifndef REQUEST_CPP
#define REQUEST_CPP

#include "request.h"
#include "../utils/ioutils.h"

#include <string>
#include <map>
#include <iostream>
#include <sstream>

http::request::request() : type(""), protocol(""), request_url(""), request_uri(""), request_query_string("")
{ }

http::request* http::request::parse (std::string str)
{
	request* req = new request();
	req->request_body = str;
	
	std::stringstream ss(str);
	std::string line;
	int line_num = 0;
	
	while (utils::ioutils::getline(ss, line) && line.size() > 0)
	{
		++line_num;
		std::stringstream lineStream(line);
		
		if (line_num == 1)
		{
			/**
			 * parsing the request line, e.g. "GET /index.html HTTP/1.1"
			 */
			std::getline (lineStream, req->type, ' ');
			std::getline (lineStream, req->request_uri, ' ');
			
			unsigned pos = req->request_uri.find('?');
			if ( pos != std::string::npos)
			{
				req->request_query_string = req->request_uri.substr (pos);
				req->request_uri = req->request_uri.substr(0, pos);
				
				std::stringstream qss(req->request_query_string.substr(1));
				std::string param;
				while ( std::getline(qss, param, '&') )
				{
					std::string key, val;
					
					key = param.substr(0, param.find('='));
					val = param.substr (param.find('=') + 1);
					
					req->query_string_map.insert(std::pair<std::string,std::string>(key,val));
				}
			}
			
			utils::ioutils::getline (lineStream, req->protocol);
		}
		else
		{
			/**
			 * parsing request headers
			 */
			std::string key, value;
			std::getline (lineStream, key, ':');
			std::getline (lineStream, value);
			
			if ( value[0] == ' ')
				value.erase (0, 1);
			
			req->header(key, value);
		}
	}
	
	std::string host = req->header("Host");
	
	if (host == "")
		return NULL;
	
	req->request_url = std::string("http://");
	req->request_url.append (host);
	
	return req;
}


http::request::~request()
{

}

std::string http::request::status_line()
{
	std::string statusline;
	std::stringstream ss(request_body);
	utils::ioutils::getline (ss, statusline);
	return statusline;
}

std::string http::request::url()
{
	return request_url;
}

std::string http::request::uri()
{
	return request_uri;
}

std::string http::request::query_string()
{
	return request_query_string;
}

std::string http::request::query_string(std::string k, std::string def)
{
	std::map<std::string,std::string>::iterator it = query_string_map.find(k);
	
	if (it != query_string_map.end())
		return it->second;
	return def;
}

std::string http::request::header (std::string k)
{
	std::map<std::string,std::string>::iterator it = headers.find(k);
	
	if (it != headers.end())
		return it->second;
	return "";
}

void http::request::header(std::string k,std::string v)
{
	headers.insert (std::pair<std::string,std::string>(k, v));
}

std::string http::request::body()
{
	return request_body;
}

#endif