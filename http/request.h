#ifndef REQUEST_H
#define REQUEST_H 

#include <map>
#include <string>

namespace http
{
	typedef enum{GET, POST} requestType;
	
	class request
	{
		private:
			// requestType type;
			std::string type;
			std::string protocol;
			
			std::map<std::string,std::string> headers;
			std::map<std::string,std::string> query_string_map;
			
			std::string request_body;
			std::string request_url;
			std::string request_uri;
			std::string request_query_string;
		
			// private constructor
			request();
		public:
			static request* parse (std::string);
			~request();
			
			std::string status_line();
			std::string url();
			std::string uri();
			std::string query_string();
			std::string query_string(std::string, std::string);
			
			std::string header (std::string);
			void header (std::string,std::string);
			std::string body();
	};
};

#endif