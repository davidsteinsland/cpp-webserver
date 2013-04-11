#ifndef IOUTILS_CPP
#define IOUTILS_CPP 

#include "ioutils.h"
#include <fstream>
#include <cerrno>
#include <string>

std::string utils::ioutils::get_file_contents (std::string filename)
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

#endif