#ifndef HTTP_SENDING_H
#define HTTP_SENDING_H

#include <string>

namespace net{class Session;}

namespace core{
	using std::string;	

	string create_http_request(int http_code, const string& mime, size_t body_size);
	void send_http_request(net::Session& session, int http_code, const string& file_path);
}

#endif//HTTP_SENDING_H
