/**
 * IOUtils
 */
#ifndef IOUTILS_H
#define IOUTILS_H 

#include <string>
#include <sstream>
 
namespace utils
{
	class ioutils
	{
		public:
			static std::stringstream& getline (std::stringstream &ss, std::string &line);

	};
};

#endif