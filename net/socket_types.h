/*
 * Cross-platform socket type definitions
 */
#ifndef SOCKET_TYPES_H
#define SOCKET_TYPES_H

#ifdef _WIN32
	#include <winsock2.h>
	#define ERRNO GetLastError()
	typedef int socklen_t;
#else
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <cerrno>
	
	#define ERRNO errno
	#define INVALID_SOCKET (-1)
	#define SOCKET_ERROR (-1)
	
	typedef int SOCKET;
#endif

#endif