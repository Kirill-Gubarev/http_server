#include "file_cacher.h"

#include <filesystem>
#include <iostream>

file::File_cacher::File_cacher(size_t max_size): max_size(max_size), used_size(0){}
file::File_cacher::File_access::File_access(std::unique_ptr<File_data>&& u_ptr, size_t count):
	u_ptr(std::move(u_ptr)), count(count){}

const file::File_data* file::File_cacher::get_file_ptr(const string& file_path){
	auto it = file_map.find(file_path);
	if(it != file_map.end()){
		it->second.count++;
		return it->second.u_ptr.get();
	}
	else
		return read_file(file_path);	
}
const file::File_data* file::File_cacher::read_file(const string& file_path){
	if(!std::filesystem::exists(file_path))
		return nullptr;

	if(!std::filesystem::is_regular_file(file_path))
		return nullptr;

	size_t file_size = std::filesystem::file_size(file_path);
	size_t available_size = max_size - used_size;
	if(file_size > available_size)
		release_space(file_size - available_size);

	std::unique_ptr<File_data> file_ptr = std::make_unique<File_data>();
	if(!read_file_full(file_ptr.get(), file_path)) 
		return nullptr;

	used_size += file_ptr->content.size();
	const auto [it, inserted] = file_map.emplace(file_path, File_access(std::move(file_ptr), 0));
	return it->second.u_ptr.get();
}
void file::File_cacher::release_space(size_t release_size){
	std::cout << "\033[33mcall file::File_cacher::release_space: "<< release_size << "\033[0m" << std::endl;
	while(release_size < max_size - used_size){
		auto min_it = file_map.begin();
		for(auto it = file_map.begin(); it != file_map.end(); ++it){
			if(min_it->second.count > it->second.count)
				min_it = it;
		}
		used_size -= min_it->second.u_ptr.get()->content.size();
		std::cout << "\033[33mreleased: "<< min_it->second.u_ptr.get()->content.size() <<"\033[0m" << std::endl;
		file_map.erase(min_it);
	}
}
