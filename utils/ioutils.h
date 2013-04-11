/**
 * IOUtils
 */
#ifndef IOUTILS_H
#define IOUTILS_H 

#include <map>
#include <string>
 
namespace utils
{
	class ioutils
	{
		private:
			static std::map<std::string,std::string> mime_types;
			
		public:
			static std::string get_file_contents (std::string) throw (int);
			static std::string get_mime_type (std::string); 
	};
};

#endif