#ifndef IOUTILS_CPP
#define IOUTILS_CPP

#include "utils/ioutils.h"
#include <string>
#include <sstream>

std::stringstream& utils::ioutils::getline (std::stringstream &ss, std::string &line)
{
	std::getline(ss, line, '\n');
	
	if (line[line.size() - 1] == '\r')
		line.resize(line.size() - 1);
	
	return ss;
}

#endif