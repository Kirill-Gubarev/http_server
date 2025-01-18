#ifndef CORE_H
#define CORE_H

#include <string>

namespace core{
	using std::string;

	string read_file(string file_path);
	string get_description(int code);
	string get_file_extension(const string& file_path);
	string get_mime_file_type(const string& file_path);
}

#endif//CORE_H
