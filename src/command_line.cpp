#include "command_line.h"
#include <string>
#include <iostream>
#include <cstdlib>

utils::Command_line::Command_line(net::Server& server):
	server(server), is_running(false){
	commands_map_init();	
}
void utils::Command_line::commands_map_init(){
	commands_map = {
		{"stop", [this](const char* command)->int{
			server.stop();
			return 0;
		}},
		{"exit", [this](const char* command)->int{
			server.stop();
			stop();
			exit(0);
		}},
		{"echo", [this](const char* command)->int{
			std::cout << command << std::endl;
			return 0;
		}},
	};
}

utils::Command_line::~Command_line(){}

void utils::Command_line::run(){
	is_running = true;

	std::string command = "";
	while(std::getline(std::cin, command) && is_running){
		std::string first_word = get_first_word(command);
		auto entry = commands_map.find(first_word);

		if(entry != commands_map.end())
			handle_command(entry->second, command.data() + first_word.length());
		else
			std::cout << "command not found" << std::endl;
	}
}
void utils::Command_line::stop(){
	is_running = false;
}

void utils::Command_line::handle_command(std::function<int(const char*)> func, const char* command){
	skip_spaces(command);
	func(command);		
}
std::string utils::Command_line::get_first_word(const std::string& str){
	size_t pos = str.find(' ');
	if(pos == std::string::npos)
		return str;
	return str.substr(0, pos);	
}
void utils::Command_line::skip_spaces(const char*& ptr){
	while(ptr[0] == ' ') 
		++ptr;
}
