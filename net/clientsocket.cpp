#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP

#include "net/clientsocket.h"
#include "http/response.h"

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#define DEFAULT_BUFFER_LENGTH 128

net::clientsocket::~clientsocket()
{
	close();
}

int net::clientsocket::read_fully (std::string &content)
{
	char buf[DEFAULT_BUFFER_LENGTH];
	int bytes_read = 0;
	int recv_result = 0;
	
	do
	{
		recv_result = recieve(buf, DEFAULT_BUFFER_LENGTH);
		
		if ( recv_result > 0 )
		{
			bytes_read += recv_result;
			
			content.append (std::string(buf, 0, bytes_read));
			
			/*
			 * 1. Check for CRLF + CRLF
			 *   1.1 If found, check content for any Content-Length header
			 *   1.1.1 If found, read additional X bytes
			 *  1.2 If not found, end request
			 * 2. Continue with reading
			 */
			unsigned headers_end_pos = content.find("\r\n\r\n");
			if ( headers_end_pos == std::string::npos )
				continue;
			
			unsigned content_length_pos = content.find ("Content-Length:");
			if ( content_length_pos == std::string::npos)
				break;
			/*
				makes sure we haven't read the content body yet.
				we know how large the content-body is, by content-length.
					content_body_s = int(content-length);
				we can also find the size of the headers:
					headers_s = content.substr(0, pos(\r\n\r\n)).length();
				we then have the following equation:
					request_s = content_body_s + headers_s
				we know also how many bytes we have read already (bytes_read):
					remaining_s = request_s - bytes_read
								= content_body_s + headers_s - bytes_read
			*/
			std::string headers = content.substr(0, headers_end_pos);
			
			std::istringstream fstr (content.substr (content_length_pos));
			std::string content_length_header;
			std::getline ( fstr, content_length_header );
			
			int headers_length = headers.length();
			int content_length = atoi(content_length_header.substr (16).c_str());
			
			int remaining_bytes = headers_length + content_length - bytes_read;
			if (remaining_bytes <= 0)
				break;
		}
		else if ( recv_result == 0 )
		{
			return 0;
		}
		else
		{
			return -1;
		}
	} while( recv_result > 0 );
	
	return bytes_read;
}

int net::clientsocket::recieve(char* buf, int len)
{
	return ::recv(socket, buf, len, 0);
}

int net::clientsocket::send(http::response* res)
{
	std::string headersString = res->status_line();
	std::map<std::string,std::string> headers = res->headers();
	
	for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		headersString.append (it->first + ": " + it->second + "\r\n");
	headersString.append("\r\n");
	
	return send (headersString + res->body());
}

int net::clientsocket::send (std::string data)
{
	return send (data.c_str(), data.length(), 0);
}

int net::clientsocket::send(const char* buf,int len,int flags)
{
	#ifdef _WIN32
		// WSAEVENT e = WSACreateEvent();
		// WSAEventSelect (socket, e, FD_READ);
		shutdown (socket, SD_RECEIVE);
	#endif
	
	return ::send (socket, buf, len, flags);
}


#endif