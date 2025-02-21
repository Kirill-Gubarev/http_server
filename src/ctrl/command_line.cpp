#include "command_line.h"

#include "net/network_engine.h"
#include "net/session_manager.h"

#include <iostream>
#include <charconv>

ctrl::Command_line::Command_line(core::Server_context& context):
	context(context), is_running(false){
	commands_map_init();	
}
void ctrl::Command_line::commands_map_init(){
	commands_map = {
		{"start", [this](std::istringstream& command)->int{
			string port_str;
			uint16_t port;
			command >> port_str;
			auto [ptr, ec] = std::from_chars(port_str.data(), port_str.data() + port_str.size(), port);
			if(ec == std::errc())
				context.network_engine.async_start(port);
			else
				context.network_engine.async_start(80);
			return 0;
		}},
		{"stop", [this](std::istringstream& command)->int{
			context.network_engine.stop();
			return 0;
		}},
		{"restart", [this](std::istringstream& command)->int{
			string port_str;
			uint16_t port;
			command >> port_str;
			auto [ptr, ec] = std::from_chars(port_str.data(), port_str.data() + port_str.size(), port);
			if(ec == std::errc())
				context.network_engine.restart(port);
			else
				context.network_engine.restart(80);
			return 0;
		}},
		{"exit", [this](std::istringstream& command)->int{
			context.network_engine.stop();
			stop();
			return 0;
		}},
		{"sessions", [this](std::istringstream& command)->int{
			string arg;
			command >> arg;
			if(arg == "-c"){
				context.session_manager.clear();
				std::cout << "all sessions has been deleted\n";
			}
			else{
				std::cout << context.session_manager.size() << '\n';
			}
			return 0;
		}},
		{"echo", [this](std::istringstream& command)->int{
			std::cout << command.str() << std::endl;
			return 0;
		}},
	};
}

ctrl::Command_line::~Command_line(){
	stop();
}

void ctrl::Command_line::start(){
	is_running = true;

	std::string input = "";
	while(is_running && std::getline(std::cin, input)){
		std::istringstream command(input);
		string first_word;
		command >> first_word;
		auto it = commands_map.find(first_word);

		if(it != commands_map.end())
			it->second(command);
		else
			std::cout << "command not found" << std::endl;
	}
}
void ctrl::Command_line::stop(){
	is_running = false;
}
