#include "core.h"
#include <unordered_map>

namespace core{
const std::unordered_map<int, string> http_codes_map{
	{200, "OK"},
	{400, "Bad Request"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"}
};

string get_description(int code){
	auto it = http_codes_map.find(code);
	if(it == http_codes_map.end())
		return "_";
	return it->second;
}

}
