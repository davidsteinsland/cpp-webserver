#ifndef MODULE_CPP
#define MODULE_CPP

#include "../module.h"

#include <windows.h>

webserver::module::module (const char* module)
{
	this->library = LoadLibrary(module);
	if (this->library == NULL)
	{
		throw ("Could not load module");
	}
}

webserver::module::~module()
{
	FreeLibrary((HMODULE)this->library);
}

void* webserver::module::call(const char* libName)
{
	FARPROC initializer = GetProcAddress((HMODULE)this->library, "handle_request");
	
	if (initializer == NULL)
	{
	   throw ("Could not call method: " + GetLastError());
	}
	
	return (void*)initializer;
}

#endif