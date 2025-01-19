#include "utils/file_data.h"

#include <fstream>

using std::string;

#define MB_SIZE (1024 * 1024)
#define MAX_FILE_SIZE (10 * MB_SIZE) // 10 MB

utils::file_data::file_data(){}
utils::file_data::file_data(const string& content, const string& extension):
	content(content), extension(extension){}
utils::file_data::file_data(string&& content, const string& extension):
	content(std::move(content)), extension(extension){}

void utils::read_file_full(file_data* const file, const string& file_path, std::error_code& ec){
	std::ifstream file_stream(file_path, std::ios::binary | std::ios::ate);
	if(!file_stream.is_open()){
		ec = std::make_error_code(std::errc::io_error);
		return;
	}

	std::streamsize file_size = file_stream.tellg();
	if(file_size > MAX_FILE_SIZE){
		ec = std::make_error_code(std::errc::file_too_large);
		return;
	}

	file_stream.seekg(0, std::ios::beg);
	file->content.resize(file_size);
	file_stream.read(&file->content[0], file_size);

	ec.clear();
	file->extension = get_file_extension(file_path);
    return;
}

string utils::get_file_extension(const string& file_path){
	size_t ext_pos = file_path.find_last_of(".") + 1;
	if (ext_pos != std::string::npos)
    	return file_path.substr(ext_pos);
	else
		return "";
}
