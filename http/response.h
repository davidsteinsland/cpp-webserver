#ifndef RESPONSE_H
#define RESPONSE_H 

#include <string>
#include <map>

namespace http
{
	class response
	{
		private:
			static std::map<std::string,std::string> status_codes;
			int status_code;
			std::string content_type;
			std::string content_body;
			
			static std::map<std::string,std::string> create_status_lines()
			{
				std::map<std::string,std::string> m;
				m["200"] = "OK";
				m["201"] = "Created";
				m["202"] = "Accepted";
				m["203"] = "Non-Authoritative Information";
				m["204"] = "No Content";
				m["205"] = "Reset Content";
				m["206"] = "Partial Content";
				m["301"] = "Moved Permanently";
				m["302"] = "Found";
				m["304"] = "Not Modified";
				m["400"] = "Bad Request";
				m["401"] = "Unauthorized";
				m["403"] = "Forbidden";
				m["404"] = "Not Found";
				m["500"] = "Internal Server Error";
				m["502"] = "Bad Gateway";
				
				return m;
			};
		public:
			response (int http_code, std::string type)
			{
				status_code = http_code;
				content_type = type;
			};
			
			void set_body(std::string body)
			{
				content_body = body;
			};
			
			int code()
			{
				return status_code;
			}
			
			std::string content()
			{
				return content_body;
			}
			
			std::string status()
			{
				return status_codes.find("" + status_code)->second;
			}
	};
};

#endif