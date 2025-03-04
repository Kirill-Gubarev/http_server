#include "http/http_utils.h"

#include <unordered_map>

namespace http{

static const std::unordered_map<int, string> http_codes_map{
	{200, "OK"},
	{400, "Bad Request"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"}
};
static const std::unordered_map<string, string> mime_types_map{
	{".html", "text/html"},
	{".js", "application/javascript"},
	{".ico", "image/x-icon"},
	{".webm", "video/webm"}
};

string get_description(int http_code){
	auto it = http_codes_map.find(http_code);
	if(it == http_codes_map.end())
		return "";
	return it->second;
}

string get_mime(const string& file_extension){
	auto it = mime_types_map.find(file_extension);
	if(it == mime_types_map.end())
		return "";
	return it->second;
}

}// namespace http
