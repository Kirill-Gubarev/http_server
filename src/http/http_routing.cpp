#include "http/http_routing.h"

#include <unordered_map>

namespace http{
	static std::unordered_map<string, string> routes = {
		{"", "index.html"}
	};
}

int http::get_route_file_path(const string& request_path, string* file_path){
	auto it = routes.find(request_path);
	if(it == routes.end())
		return 0;
	*file_path = it->second;	
	return 1;
}

