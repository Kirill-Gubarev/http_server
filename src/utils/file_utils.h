#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

namespace utils{
	using std::string;

	string get_file_extension(const string& file_path);
	string read_file_full(const string& file_path);
}

#endif//FILE_UTILS_H
