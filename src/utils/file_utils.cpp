#include "utils/file_utils.h"

#include <fstream>
#include <iostream>

#define MB_SIZE (1024 * 1024)
#define MAX_FILE_SIZE (10 * MB_SIZE) // 10 MB

namespace utils{

string get_file_extension(const string& file_path){
	size_t dot_pos = file_path.find_last_of(".");
	if (dot_pos != std::string::npos) 
    	return file_path.substr(dot_pos);
	else
		return "";
}
string read_file_full(const string& file_path){
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

}// namespace utils
