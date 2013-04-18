/**
 * Config header
 * @TODO Move configuration variables to a file.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace config
{
	const std::string HTML_ROOT = "./public_html";
	const std::string MODULES_ROOT = "./modules";
	const int PORT = 80;
	const size_t NUM_WORKER_THREADS = 5;
	
	#ifdef _WIN32
		const std::string MODULE_EXT = ".dll";
	#else
		const std::string MODULE_EXT = ".so";
	#endif
};

#endif