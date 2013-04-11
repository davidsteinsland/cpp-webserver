#ifndef RESPONSE_H
#define RESPONSE_H 

#include <string>
#include <map>

namespace http
{
	class response
	{
		private:
			static std::map<int,std::string> status_codes;
			
			std::map<std::string,std::string> response_headers;
			int status_code;
			
			std::string server_protocol;
			std::string content_type;
			std::string content_body;
			
			static void create_status_lines();
		public:
			response (int,std::string);
			~response();
			
			void set_header (std::string,std::string);
			void set_body(std::string);
			
			std::map<std::string,std::string> headers();
			std::string response_line();
			std::string protocol();
			int code();
			std::string body();
			std::string status();
	};
};

#endif