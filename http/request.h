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
			/**
			 * Parses a complete HTTP Request and returns a request object
			 */
			static request* parse (std::string);
			/**
			 * Parses a query string and returns a map of the key-value pairs
			 */
			static std::map<std::string,std::string> parse_query_string (std::string);
			~request();
			
			/**
			 * Returns the status line of the request, e.g. GET / HTTP/1.1
			 */
			std::string status_line();
			/**
			 * Returns the url of the request, e.g. http://localhost/
			 */
			std::string url();
			/**
			 * Returns the uri of the request, e.g. /path/to/file?foo=bar
			 */
			std::string uri();
			/**
			 * Returns the file requested
			 */
			std::string file();
			/**
			 * Returns which protocol was used when connecting
			 */
			std::string protocol();
			/**
			 * Returns the request method (POST, GET, ...)
			 */
			std::string method();
			/**
			 * Returns the query string
			 */
			std::string query_string();
			/**
			 * Returns the value of given key, or a default value if it cannot be found
			 */
			std::string query_string(std::string, std::string);
			
			/**
			 * Returns all request headers as a map
			 */
			std::map<std::string,std::string> headers(); 
			/**
			 * Returns the value of a header
			 */
			std::string header (std::string);
			/**
			 * Sets a header value
			 */
			void header (std::string,std::string);
			/**
			 * Returns the body of the request
			 */
			std::string body();
	};
};

#endif