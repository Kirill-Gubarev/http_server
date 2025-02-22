#include "command_line.h"

#include <iostream>

ctrl::Command_line::Command_line(core::Server_context& context):
	context(context), is_running(false){
	commands_map_init();	
}

ctrl::Command_line::~Command_line(){
	stop();
}

void ctrl::Command_line::start(){
	is_running = true;
	std::string input = "";
	while(is_running && std::getline(std::cin, input)){
		utils::Str_handler command(input);

		auto it = commands_map.find(command.next_word());
		if(it != commands_map.end())
			it->second(command);
		else
			std::cout << "command not found" << std::endl;
	}
}
void ctrl::Command_line::stop(){
	is_running = false;
}
