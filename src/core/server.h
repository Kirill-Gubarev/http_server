#ifndef SERVER_H
#define SERVER_H

#include "core/server_context.h"
#include "ctrl/command_line.h"
#include "net/network_engine.h"
#include "net/session_manager.h"
#include "http/http_handler.h"
#include "html/html_renderer.h"
#include "file/file_cacher.h"

#include <cstdint>

namespace core{
	class Server{
	friend class Server_context;

	private:
		bool is_running = false;

		ctrl::Command_line command_line;
		net::Network_engine network_engine;
		net::Session_manager session_manager;		
		http::Http_handler http_handler;
		html::Html_renderer html_renderer;
		file::File_cacher file_cacher;
		Server_context context;

	public:
		explicit Server();
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
		~Server();

		void start(uint16_t port);
		void stop();
	};
}

#endif//SERVER_H
