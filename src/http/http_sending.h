#ifndef HTTP_SENDING_H
#define HTTP_SENDING_H

#include <string>

namespace net{class Session;}

namespace http{
	using std::string;	
	
	void send_http_request(net::Session& session, int http_code, const string& request_path);
	void send_error_http_request(net::Session& session, int http_code);
}

#endif//HTTP_SENDING_Hs
