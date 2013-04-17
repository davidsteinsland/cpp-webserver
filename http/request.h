#ifndef REQUEST_H
#define REQUEST_H 

#include <map>
#include <string>

namespace http
{
	typedef enum{GET, POST} requestMethod;
	
	class request
	{
		private:
			// requestType type;
			std::string request_method;
			std::string server_protocol;
			
			std::map<std::string,std::string> request_headers;
			std::map<std::string,std::string> query_string_map;
			
			std::string raw_message;
			std::string request_body;
			std::string request_url;
			std::string request_uri;
			std::string file_name;
			std::string request_query_string;
		
			// private constructor
			request();
		public:
			static request* parse (std::string);
			static std::map<std::string,std::string> parse_query_string (std::string);
			~request();
			
			std::string status_line();
			std::string url();
			std::string uri();
			std::string file();
			std::string protocol();
			std::string method();
			std::string query_string();
			std::string query_string(std::string, std::string);
			
			std::map<std::string,std::string> headers(); 
			std::string header (std::string);
			void header (std::string,std::string);
			std::string body();
	};
};

#endif