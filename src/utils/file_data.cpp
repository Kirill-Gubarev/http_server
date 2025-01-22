#include "utils/file_data.h"

#include "def/default_definitions.h"

#include <fstream>
#include <filesystem>

#define MAX_FILE_SIZE (10*MB)

using std::string;

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
	return std::filesystem::path(file_path).extension().string();
}
