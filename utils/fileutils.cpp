#ifndef FILEUTILS_CPP
#define FILEUTILS_CPP 

#include "utils/fileutils.h"
#include <fstream>
#include <cerrno>
#include <string>
#include <sstream>
#include <map>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

std::map<std::string,std::string> utils::fileutils::mime_types;

std::string utils::fileutils::contents (std::string filename)
	throw (int)
{
	std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		
		return contents;
	}
	
	throw(errno);
}

std::string utils::fileutils::content_type (std::string key, std::string def)
{
	if (mime_types.size() == 0)
	{
		std::ifstream in("./mimes.list", std::ios::in);
	
		if (in)
		{
			std::string line;
			while ( std::getline (in, line) )
			{
				std::string ext, mime_type;
				
				std::stringstream ss(line);
				std::getline(ss, ext, '\t');
				std::getline(ss, mime_type, '\t');
				
				mime_types.insert (std::pair<std::string,std::string> (ext, mime_type));
				
			}
		}
	}
	
	std::map<std::string,std::string>::iterator it = mime_types.find(key);
				
	if (it != mime_types.end())
		return it->second;
	return def;
}

bool utils::fileutils::is_file (std::string filename)
{
	if (is_directory (filename))
		return false;

	std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
	if (in)
	{
		in.close();
		
		return true;
	}
	
	return false;
}

bool utils::fileutils::is_directory (std::string path)
{
	#ifdef _WIN32
		DWORD ftyp = GetFileAttributesA(path.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;
		return false;
	#else
	struct stat st;
	if (stat(path.c_str(), &st) == 0 && S_ISDIR (st.st_mode))
		return true;
	#endif

	return false;
}

#endif