#include "command_line.h"

#include "net/network_engine.h"
#include "net/session_manager.h"

#include <string>
#include <iostream>

ctrl::Command_line::Command_line(core::Server_context& context):
	context(context), is_running(false){
	commands_map_init();	
}
void ctrl::Command_line::commands_map_init(){
	commands_map = {
		{"start", [this](const char* command)->int{
			context.network_engine.async_start();
			return 0;
		}},
		{"stop", [this](const char* command)->int{
			context.network_engine.stop();
			return 0;
		}},
		{"restart", [this](const char* command)->int{
			context.network_engine.restart();
			return 0;
		}},
		{"exit", [this](const char* command)->int{
			context.network_engine.stop();
			stop();
			return 0;
		}},
		{"sessions", [this](const char* command)->int{
			std::cout << context.session_manager.size() << std::endl;
			return 0;
		}},
		{"clear", [this](const char* command)->int{
			context.session_manager.clear();
			return 0;
		}},
		{"echo", [this](const char* command)->int{
			std::cout << command << std::endl;
			return 0;
		}},
	};
}

ctrl::Command_line::~Command_line(){
	stop();
}

void ctrl::Command_line::start(){
	is_running = true;

	std::string command = "";
	while(is_running && std::getline(std::cin, command)){
		std::string first_word = get_first_word(command);
		auto entry = commands_map.find(first_word);

		if(entry != commands_map.end())
			handle_command(entry->second, command.data() + first_word.length());
		else
			std::cout << "command not found" << std::endl;
	}
}
void ctrl::Command_line::stop(){
	is_running = false;
}

void ctrl::Command_line::handle_command(std::function<int(const char*)> func, const char* command){
	skip_spaces(command);
	func(command);		
}
std::string ctrl::Command_line::get_first_word(const std::string& str){
	size_t pos = str.find(' ');
	if(pos == std::string::npos)
		return str;
	return str.substr(0, pos);	
}
void ctrl::Command_line::skip_spaces(const char*& ptr){
	while(ptr[0] == ' ') 
		++ptr;
}
