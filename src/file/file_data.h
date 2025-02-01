#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <string>

namespace file{
	using std::string;

	struct File_data{
		string path;
		string content;
		string extension;

		File_data();
		File_data(const File_data& other);
		File_data(const string& path, const string& content, const string& extension);
		File_data(const string& path, string&& content, const string& extension);
		File_data& operator=(const File_data& other);
	};

	File_data* read_file_full(File_data* file, const string& file_path);
	string get_file_extension(const string& file_path);
}

#endif//FILE_DATA_H
