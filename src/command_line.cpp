#include "command_line.h"
#include <string>
#include <iostream>

utils::Command_line::Command_line(net::Server& server):
	server(server){
	commands_map_init();	
}
void utils::Command_line::commands_map_init(){
	commands_map = {
		{"stop", [](const std::string& command, net::Server& server)->int{server.stop(); return 0;}},
	};
}

utils::Command_line::~Command_line(){}

void utils::Command_line::run(){
	std::string buf = "";
	while(std::getline(std::cin, buf)){
		auto entry = commands_map.find(buf);
		if(entry != commands_map.end())
			entry->second(buf, server);		
		else
			std::cout << "command not found" << std::endl;
	}
}
