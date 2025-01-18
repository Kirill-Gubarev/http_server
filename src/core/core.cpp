#include "core.h"

#include <unordered_map>
#include <fstream>
#include <iostream>

#define MB_SIZE (1024 * 1024)
#define MAX_FILE_SIZE (10 * MB_SIZE) // 10 MB

using std::string;
static const std::unordered_map<int, string> http_codes_map{
	{200, "OK"},
	{400, "Bad Request"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"}
};
static const std::unordered_map<string, string> mime_file_types_map{
	{".html", "text/html"},
	{".js", "application/javascript"},
	{".ico", "image/x-icon"}
};
string core::read_file(string file_path){
	std::ifstream file(file_path, std::ios::binary | std::ios::ate);
	if(!file.is_open()){
		std::cout << "\033[31mfile opening error: " + file_path + "\033[0m\n";	
		return "";
	}
	std::streamsize file_size = file.tellg();

	if(file_size > MAX_FILE_SIZE){
        std::cout 
			<< "\033[31mfile size exceeds the limit: (" << file_size / MB_SIZE
			<< " MB, max: " << MAX_FILE_SIZE / MB_SIZE 
			<< " MB)\033[0m\n";
		return "";
	}

	file.seekg(0, std::ios::beg);

	string buffer(file_size, '\0');
	if (!file.read(&buffer[0], file_size)) {
        std::cout << "\033[31mfile reading error: " << file_path << "\033[0m\n";
        return "";
    }

    return buffer;
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
