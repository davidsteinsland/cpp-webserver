#ifndef MODULE_H
#define MODULE_H

namespace webserver
{
	class module
	{
		private:
			void* library;
		public:
			module(const char*);
			~module();
			
			void* call(const char*);
	};
}
#endif