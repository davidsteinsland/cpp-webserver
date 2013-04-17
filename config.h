#include <string>

namespace config
{
	const std::string HTML_ROOT = "./public_html";
	const std::string MODULES_ROOT = "./modules";
	const int PORT = 80;
	const int NUM_WORKER_THREADS = 3;
	
	#ifdef _WIN32
		const std::string MODULE_EXT = ".dll";
	#else
		const std::string MODULE_EXT = ".so";
	#endif
};