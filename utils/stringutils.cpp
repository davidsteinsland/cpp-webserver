#ifndef STRINGUTILS_CPP
#define STRINGUTILS_CPP

#include "utils/stringutils.h"

#include <cstdio>
#include <string>
#include <sstream>

std::string utils::stringutils::urldecode (std::string &url)
{
	std::string ret;
	char ch;
	unsigned i;
	int j;
	for (i = 0; i < url.length(); i++)
	{
		if (int (url[i]) == 37)
		{
			sscanf (url.substr (i + 1, 2).c_str(), "%x", &j);
			ch = static_cast<char>(j);
			ret += ch;
			i = i + 2;
		}
		else if (url[i] == '+')
			ret += ' ';
		else
			ret += url[i];
	}
	
	return ret;
}

#endif