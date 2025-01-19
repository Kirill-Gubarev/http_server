#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <string>

namespace http{
	using std::string;

	string get_description(int http_code);
	string get_mime(const string& file_path);
}

#endif//HTTP_UTILS
