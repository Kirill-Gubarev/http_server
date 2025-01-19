#ifndef HTTP_SENDING_H
#define HTTP_SENDING_H

#include <string>

namespace net{class Session;}

namespace http{
	using std::string;	
	
	inline void send_message(net::Session& session, const string& message);
	void send_file_chunked(net::Session& session, string file_path);
	void send_file_full(net::Session& session, const string& file);
	void send_http_request(net::Session& session, int http_code, const string& file_path);
}

#endif//HTTP_SENDING_H
