#include "command_line.h"
#include <string>
#include <iostream>

utils::Command_line::Command_line(net::Server& server):
	server(server), is_running(false){
	commands_map_init();	
}
void utils::Command_line::commands_map_init(){
	commands_map = {
		{"stop", 
			[this](const std::string& command)->int{
				server.stop();
				stop();
				return 0;
			}
		},
	};
}

utils::Command_line::~Command_line(){}

void utils::Command_line::run(){
	is_running = true;

	std::string command = "";
	while(std::getline(std::cin, command) && is_running){
		auto entry = commands_map.find(command);
		if(entry != commands_map.end())
			entry->second(command);		
		else
			std::cout << "command not found" << std::endl;
	}
}
void utils::Command_line::stop(){
	is_running = false;
}
