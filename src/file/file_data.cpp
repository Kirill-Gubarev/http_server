#include "file/file_data.h"

#include "def/default_definitions.h"

#include <fstream>
#include <filesystem>
#include <iostream>

#define MAX_FILE_SIZE (10*MB)

using std::string;

file::File_data::File_data(){}
file::File_data::File_data(const File_data& other):
	path(other.path), content(other.content), extension(other.extension){}
file::File_data::File_data(const string& path, const string& content, const string& extension):
	path(path), content(content), extension(extension){}
file::File_data::File_data(const string& path, string&& content, const string& extension):
	path(path), content(std::move(content)), extension(extension){}
file::File_data& file::File_data::File_data::operator=(const File_data& other){
	path = other.path;
	content = other.content;
	extension = other.extension;
	return *this;
}

file::File_data* file::read_file_full(File_data* file_ptr, const string& file_path){
	std::cout << "\033[33mcall file::read_file_full: "<< file_path << "\033[0m" << std::endl;
	std::ifstream file_stream(file_path, std::ios::binary | std::ios::ate);
	if(!file_stream.is_open()) return nullptr;

	std::streamsize file_size = file_stream.tellg();
	if(file_size > MAX_FILE_SIZE) return nullptr;

	file_stream.seekg(0, std::ios::beg);
	file_ptr->content.resize(file_size);
	file_stream.read(file_ptr->content.data(), file_size);

	file_ptr->path = file_path;
	file_ptr->extension = get_file_extension(file_path);
	return file_ptr;
}

string file::get_file_extension(const string& file_path){
	return std::filesystem::path(file_path).extension().string();
}
