#ifndef RESPONSE_CPP
#define RESPONSE_CPP

#include <string>
#include <map>
#include <sstream>
#include "response.h"

std::map<int,std::string> http::response::status_codes;

void http::response::create_status_lines()
{
	status_codes[200] = "OK";
	status_codes[201] = "Created";
	status_codes[202] = "Accepted";
	status_codes[203] = "Non-Authoritative Information";
	status_codes[204] = "No Content";
	status_codes[205] = "Reset Content";
	status_codes[206] = "Partial Content";
	status_codes[301] = "Moved Permanently";
	status_codes[302] = "Found";
	status_codes[304] = "Not Modified";
	status_codes[400] = "Bad Request";
	status_codes[401] = "Unauthorized";
	status_codes[403] = "Forbidden";
	status_codes[404] = "Not Found";
	status_codes[500] = "Internal Server Error";
	status_codes[502] = "Bad Gateway";
};

http::response::response (int http_code, std::string type)
{
	this->create_status_lines();
	
	server_protocol = "HTTP/1.1";
	
	this->set_status (http_code);
	this->set_content_type(type);
}

http::response::~response()
{
	
}

void http::response::set_status(int k)
{
	status_code = k;
}

void http::response::set_content_type (std::string type)
{
	content_type = type;
	
	if (type.length() > 0)
		this->set_header ("Content-Type", type);
	else
		response_headers.erase (type);
}

void http::response::set_header (std::string k,std::string v)
{
	response_headers.insert (std::pair<std::string,std::string>(k,v));
}

void http::response::set_body(std::string body)
{
	// std::to_string() fails miserably with MinGW
	std::stringstream ss;
	ss << body.length();
	this->set_header("Content-Length", ss.str());
	content_body = body;
};

std::map<std::string,std::string> http::response::headers()
{
	return response_headers;
}

std::string http::response::status_line()
{
	std::stringstream ss;	
	ss << server_protocol << ' ' << status_code << ' ' << this->status() << "\r\n";
	
	return ss.str();
}

std::string http::response::protocol()
{
	return server_protocol;
}

int http::response::code()
{
	return status_code;
}

std::string http::response::body()
{
	return content_body;
}

std::string http::response::status()
{
	return status_codes.find(status_code)->second;
}

#endif