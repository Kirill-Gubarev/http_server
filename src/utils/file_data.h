#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <string>
#include <system_error>

namespace utils{
	using std::string;

	struct file_data{
		string content;
		string extension;

		file_data();
		file_data(const string& content, const string& extension);
		file_data(string&& content, const string& extension);

	};

	void read_file_full(file_data* const file, const string& file_path, std::error_code& ec);
	string get_file_extension(const string& file_path);
}

#endif//FILE_DATA_H
