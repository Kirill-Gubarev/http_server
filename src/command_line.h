#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "server.h"
#include <unordered_map>

namespace utils{

class Command_line{
private:
	net::Server& server;
	std::unordered_map<
		std::string, 
		int(*)(const std::string& command, net::Server& server)>
			commands_map;
	void commands_map_init();

public:
	explicit Command_line(net::Server& server);
	Command_line(const Command_line& other) = delete;
	Command_line operator=(const Command_line& other) = delete;
	~Command_line();

	void run();
};
}

#endif//COMMAND_LINE_H
