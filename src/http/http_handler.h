#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "core/server_context.h"
#include <string>

namespace http{
	using std::string;

	class Http_handler{
	private:
		core::Server_context& context;

	public:
		explicit Http_handler(core::Server_context& context);
		Http_handler(const Http_handler& other) = delete;	
		Http_handler operator=(const Http_handler& other) = delete;	
		~Http_handler();

		void process_request(net::Session& session, string&& request);

	private:
		void print_request(const string& request);

		void send_http_request(net::Session& session, int http_code, const string& request_path);
		void send_error_http_request(net::Session& session, int http_code);
	};
}

#endif//HTTP_HANDLER_H
