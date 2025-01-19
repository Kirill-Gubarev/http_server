#ifndef HTTP_SENDING_H
#define HTTP_SENDING_H

#include "utils/file_data.h"

#include <string>

namespace net{class Session;}

namespace http{
	using std::string;	
	
	inline void send_message(net::Session& session, const string& message);

	void send_file_chunked(net::Session& session, const string& file_path);
	void send_file_full(net::Session& session, const utils::file_data& file);

	void send_http_request(net::Session& session, int http_code, const string& request_path);
	void send_error_http_request(net::Session& session, int http_code);
}

#endif//HTTP_SENDING_H
