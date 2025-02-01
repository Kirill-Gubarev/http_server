#ifndef FILE_CACHER_H
#define FILE_CACHER_H

#include "file/file_data.h"

#include <unordered_map>
#include <memory>

namespace file{
	class File_cacher{
	private:
		struct File_access{
			std::unique_ptr<File_data> u_ptr;
			size_t count;
			File_access(std::unique_ptr<File_data>&& u_ptr, size_t count);
		};
		std::unordered_map<std::string, File_access> file_map;
		size_t max_size;
		size_t used_size;

	public:
		explicit File_cacher(size_t max_size);
		File_cacher(const File_cacher& other) = delete;
		File_cacher operator=(const File_cacher& other) = delete;

		const File_data* get_file_ptr(const string& file_path);

	private:
		const File_data* read_file(const string& file_path);
		void release_space(size_t release_size);
	};
}

#endif//FILE_CACHER_H
