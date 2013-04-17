#ifndef REQUEST_CPP
#define REQUEST_CPP

#include "http/request.h"
#include "utils/ioutils.h"

#include <string>
#include <map>
#include <sstream>
#include <iostream>

http::request::request() : request_method(""), server_protocol(""), request_body (""), 
						   request_url(""), request_uri(""), file_name(""), request_query_string("")
{ }

http::request* http::request::parse (std::string str)
{
	request* req = new request();
	req->raw_message = str;
	
	std::stringstream ss(str);
	std::string line;
	
	/**
	 * parsing the request line, e.g. "GET /index.html?name=hey HTTP/1.1"
	 */
	utils::ioutils::getline(ss, line);
	std::stringstream lineStream(line);
	std::getline (lineStream, req->request_method, ' ');
	std::getline (lineStream, req->request_uri, ' ');
	
	req->file_name = req->request_uri;
	
	unsigned pos;
	if (req->request_uri.length() > 1 && (pos = req->request_uri.find('?')) != std::string::npos && pos <= req->request_uri.length())
	{
		// name=hey
		req->request_query_string = req->request_uri.substr (pos + 1);
		// index.html
		req->file_name = req->request_uri.substr(0, pos);
		req->query_string_map = parse_query_string (req->request_query_string);
	}
	
	utils::ioutils::getline (lineStream, req->server_protocol);
	
	/**
	 * parsing additional request headers. Ends when no more lines can be fetched (i.e. an "\r\n" line).
	 */
	while (utils::ioutils::getline(ss, line) && line.size() > 0)
	{
		std::stringstream headerStream(line);
		std::string key, value;
		std::getline (headerStream, key, ':');
		std::getline (headerStream, value);
		
		if ( value[0] == ' ')
			value.erase (0, 1);
		
		req->header(key, value);
	}
	
	/**
	 * the next line should give us the query string, if method equals POST
	 */
	utils::ioutils::getline(ss, line);
	if (req->method() == "POST" && line.length() > 0)
	{
		req->request_query_string = line;
		req->query_string_map = parse_query_string (line);
	}
	
	std::string host = req->header("Host");
	
	if (host == "")
		return NULL;
	
	req->request_url = std::string("http://");
	req->request_url.append (host);
	
	return req;
}

std::map<std::string,std::string> http::request::parse_query_string (std::string querystring)
{
	std::map<std::string,std::string> qsmap;
	
	std::stringstream qss(querystring);
	std::string line;
	while ( std::getline(qss, line, '&') )
	{
		qsmap.insert(std::pair<std::string,std::string>(line.substr(0, line.find('=')), line.substr (line.find('=') + 1)));
	}
	
	return qsmap;
}

http::request::~request()
{

}

std::string http::request::status_line()
{
	std::string statusline;
	std::stringstream ss(raw_message);
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

std::string http::request::file ()
{
	return file_name;
}

std::string http::request::protocol()
{
	return server_protocol;
}

std::string http::request::method()
{
	return request_method;
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

std::map<std::string,std::string> http::request::headers()
{
	return request_headers;
}

std::string http::request::header (std::string k)
{
	std::map<std::string,std::string>::iterator it = request_headers.find(k);
	
	if (it != request_headers.end())
		return it->second;
	return "";
}

void http::request::header(std::string k,std::string v)
{
	request_headers.insert (std::pair<std::string,std::string>(k, v));
}

std::string http::request::body()
{
	return request_body;
}

#endif
