/**
 * IOUtils
 */
#ifndef IOUTILS_H
#define IOUTILS_H 

#include <string>
 
namespace utils
{
	class ioutils
	{
		public:
			static std::string get_file_contents (std::string) throw (int);
	};
};

#endif