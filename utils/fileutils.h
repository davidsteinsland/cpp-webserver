/**
 * FileUtils
 */
#ifndef FILEUTILS_H
#define FILEUTILS_H 

#include <map>
#include <string>
 
namespace utils
{
	class fileutils
	{
		private:
			static std::map<std::string,std::string> mime_types;
			
		public:
			static std::string contents (std::string) throw (int);
			static std::string content_type (std::string,std::string);
			static bool is_file (std::string);
			static bool is_directory (std::string);
	};
};

#endif