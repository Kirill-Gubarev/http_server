#include "http/http_routing.h"

#include <unordered_map>

namespace http{

static std::unordered_map<string, string> routes = {
	{"/", "index.html"},
	{"/favicon.ico", "favicon.ico"},
	{"/clicker", "clicker.html"},
	{"/clicker.js", "clicker.js"},
	{"/monkey.webm", "monkey.webm"}
};

string get_route_file_path(const string& request_path, std::error_code& ec){
	auto it = routes.find(request_path);
	if(it == routes.end()){
		ec = std::make_error_code(std::errc::invalid_argument);
		return "";
	}
	return it->second;	
}

}// namespce http
