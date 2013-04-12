#ifndef IOUTILS_CPP
#define IOUTILS_CPP 

#include "fileutils.h"
#include <fstream>
#include <cerrno>
#include <string>
#include <sstream>
#include <map>

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

#endif