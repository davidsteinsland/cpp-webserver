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
			
			std::string reqBody;
			std::string url;
			std::string uri;
			std::string queryString;
		
			void parse (std::string);
		public:
			request (std::string);
			~request();
			
			std::string get_url()
			{
				return url;
			}
			
			std::string get_uri()
			{
				return uri;
			}
			
			std::string get_query_string()
			{
				return queryString;
			}
			
			std::string get_header (std::string k)
			{
				return headers.find(k)->second;
			}
			
			std::string body()
			{
				return reqBody;
			}
	};
};

#endif