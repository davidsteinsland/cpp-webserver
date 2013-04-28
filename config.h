/**
 * Config header
 * @TODO Move configuration variables to a file.
 */
#ifndef CONFIG_H
#define CONFIG_H

#ifdef _WIN32
	#include <string.h> /* _strerror() */
	#include <malloc.h> /* _alloca() */
	//#define strerror(x)  _ultoa(x, (char *) _alloca(sizeof(x) *3 ), 10)
	#define WINCDECL __cdecl
#else
	#define WINCDECL
	#include <cstring> /* strerror() */
#endif

#include <string>
#include <vector>

namespace config
{
	const std::string HTML_ROOT = "./public_html";
	const std::string MODULES_ROOT = "./modules";
	const std::string ERRORS_ROOT = "./errors";
	
	const int PORT = 80;
	const size_t NUM_WORKER_THREADS = 10;
	
	#ifdef _WIN32
		const std::string MODULE_EXT = ".dll";
	#else
		const std::string MODULE_EXT = ".so";
	#endif
};

#endif