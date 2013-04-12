#ifndef FILEUTILS_CPP
#define FILEUTILS_CPP 

#include "fileutils.h"
#include <fstream>
#include <cerrno>
#include <string>
#include <sstream>
#include <map>

#ifdef _WIN32
	#include <windows.h>
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

std::string utils::fileutils::content_type (std::string key)
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
	
	return mime_types.find(key)->second;
}

bool utils::fileutils::is_file (std::string filename)
{
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
	if (stat(path,&st) == 0 && st.st_mode & S_IFDIR != 0)
		return true;
	#endif
}

#endif