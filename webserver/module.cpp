#ifndef MODULE_CPP
#define MODULE_CPP

#include "webserver/module.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <cstddef>
	#include <dlfcn.h>
#endif

webserver::module::module (const char* module)
{
	#ifdef _WIN32
	library = LoadLibrary(module);
	#else
	library = dlopen(module, RTLD_LAZY);
	#endif
	
	if (library == NULL)
	{
		throw ("Could not load module");
	}
}

webserver::module::~module()
{
	#ifdef _WIN32
	FreeLibrary((HMODULE)library);
	#else
	dlclose(library);
	#endif
}

void* webserver::module::call(const char* method)
{
	#ifdef _WIN32
	FARPROC initializer = GetProcAddress((HMODULE)library, method);
	#else
	void* initializer = dlsym(library, method);
	#endif
	
	if (initializer == NULL)
	{
	   throw ("Could not call method");
	}
	
	return (void*)initializer;
}

#endif