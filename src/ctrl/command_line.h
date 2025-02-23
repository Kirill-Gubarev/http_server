#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "core/server_context.h"
#include "utils/str_handler.h"

#include <string>
#include <unordered_map>
#include <functional>

namespace ctrl{
	using std::string;

	class Command_line{
	private:
		core::Server_context& context;
		bool is_running;
		std::unordered_map<
			std::string, 
			std::function<int(utils::Str_handler&)>> commands_map;
		void commands_map_init();

	public:
		explicit Command_line(core::Server_context& context);
		Command_line(const Command_line& other) = delete;
		Command_line& operator=(const Command_line& other) = delete;
		~Command_line();

		void run();
		void stop();
	};
}

#endif//COMMAND_LINE_H
