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
		
			void parse (std::string);
		public:
			request (std::string);
			~request();
			
			std::string status_line();
			
			std::string url()
			{
				return request_url;
			}
			
			std::string uri()
			{
				return request_uri;
			}
			
			std::string query_string()
			{
				return request_query_string;
			}
			
			std::string query_string(std::string k, std::string def)
			{
				std::map<std::string,std::string>::iterator it = query_string_map.find(k);
				
				if (it != query_string_map.end())
					return it->second;
				return def;
			}
			
			std::string header (std::string k)
			{
				return headers.find(k)->second;
			}
			
			void header (std::string,std::string);
			
			std::string body()
			{
				return request_body;
			}
	};
};

#endif