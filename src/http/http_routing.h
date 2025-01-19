#ifndef HTTP_ROUTING_H
#define HTTP_ROUTING_H

#include <string>
#include <system_error>

namespace http{
	using std::string;	
	string get_route_file_path(const string& request_path, std::error_code& ec);
}

#endif//HTTP_ROUTING_H
