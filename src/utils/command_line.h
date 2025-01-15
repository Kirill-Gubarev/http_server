#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "net/server.h"
#include <unordered_map>
#include <functional>

namespace utils{

class Command_line{
private:
	net::Server& server;
	bool is_running;
	std::unordered_map<
		std::string, 
		std::function<int(const char*)>> commands_map;
	void commands_map_init();

public:
	explicit Command_line(net::Server& server);
	Command_line(const Command_line& other) = delete;
	Command_line operator=(const Command_line& other) = delete;
	~Command_line();

	void run();
	void stop();

private:
	void handle_command(std::function<int(const char*)> func, const char* command);
	std::string static get_first_word(const std::string& str);
	void static skip_spaces(const char*& ptr);
};
}

#endif//COMMAND_LINE_H
