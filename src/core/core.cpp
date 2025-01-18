#include "core.h"
#include <unordered_map>

using std::string;
namespace core{
	const std::unordered_map<int, string> http_codes_map{
		{200, "OK"},
		{400, "Bad Request"},
		{500, "Internal Server Error"},
		{501, "Not Implemented"}
	};
	const std::unordered_map<string, string> mime_file_types_map{
		{".html", "text/html"},
		{".js", "application/javascript"},
	};
}

string core::get_description(int code){
	auto it = http_codes_map.find(code);
	if(it == http_codes_map.end())
		return "";
	return it->second;
}

string core::get_file_extension(const string& file_path){
	size_t dot_pos = file_path.find_last_of(".");
	if (dot_pos != std::string::npos) 
    	return file_path.substr(dot_pos);
	else
		return "";
}
string core::get_mime_file_type(const string& file_path){
	auto it = mime_file_types_map.find(get_file_extension(file_path));
	if(it == mime_file_types_map.end())
		return "";
	return it->second;
}
