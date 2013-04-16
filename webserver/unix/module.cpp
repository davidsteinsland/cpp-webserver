#ifndef MODUL_CPP
#define MODULE_CPP

#include "../module.h"
#include <cstddef>
#include <dlfcn.h>

webserver::module::module (const char* module)
{
	library = dlopen(module, RTLD_LAZY);
	if (library == NULL)
	{
		throw ("Could not load module");
	}
}

webserver::module::~module()
{
	dlclose(library);
}

void* webserver::module::call(const char* method)
{
	void* initializer = dlsym(library,"SDL_Init");
	if (initializer == NULL)
	{
	   throw ("Could not call method");
	}
	
	return initializer;
}

#endif
