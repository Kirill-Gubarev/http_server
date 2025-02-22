#ifndef HTTP_ROUTING_H
#define HTTP_ROUTING_H

#include <string>

namespace http{
	using std::string;	

	int get_route_file_path(const string& request_path, string* file_path);
}

#endif//HTTP_ROUTING_H
