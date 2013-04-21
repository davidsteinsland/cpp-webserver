#ifndef CLIENT_SOCKET_CPP
#define CLIENT_SOCKET_CPP

#include "net/clientsocket.h"
#include "http/response.h"

#include <string>
#include <iostream>
#include <string>
#include <map>

net::clientsocket::~clientsocket()
{
	close();
}

void net::clientsocket::close()
{
	if (socket != INVALID_SOCKET)
	{
		#ifdef _WIN32
		closesocket(socket);
		#else
		::close(socket);
		#endif
	}
}

bool net::clientsocket::valid()
{
	return socket != INVALID_SOCKET;
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
	return ::send (socket, buf, len, flags);
}


#endif