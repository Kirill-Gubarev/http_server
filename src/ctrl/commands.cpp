#include "ctrl/command_line.h"

#include "net/network_engine.h"
#include "net/session_manager.h"
#include "utils/str_handler.h"
#include "utils/str_utils.hpp"

#include <iostream>

void ctrl::Command_line::commands_map_init(){
	commands_map = {
		{"start", [this](utils::Str_handler& command)->int{
			uint16_t port;
			if(utils::parse(command.get_first_word(), &port))
				context.network_engine.async_start(port);
			else
				context.network_engine.async_start(80);
			return 0;
		}},
		{"stop", [this](utils::Str_handler& command)->int{
			context.network_engine.stop();
			return 0;
		}},
		{"restart", [this](utils::Str_handler& command)->int{
			uint16_t port;
			if(utils::parse(command.get_first_word(), &port))
				context.network_engine.restart(port);
			else
				context.network_engine.restart(80);
			return 0;
		}},
		{"exit", [this](utils::Str_handler& command)->int{
			context.network_engine.stop();
			stop();
			return 0;
		}},
		{"sessions", [this](utils::Str_handler& command)->int{
			string arg = command.get_first_word();
			if(arg == "-c"){
				context.session_manager.clear();
				std::cout << "all sessions has been deleted\n";
			}
			else{
				std::cout << context.session_manager.size() << '\n';
			}
			return 0;
		}},
		{"echo", [this](utils::Str_handler& command)->int{
			std::cout << command.str_view() << std::endl;
			return 0;
		}},
	};
}
