#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <cstdint>
#include <thread>

#include "net/session_manager.h"

namespace net{
	class Session;
	using asio::ip::tcp;

	struct Server_context{
		Session_manager session_manager;		
	};
	class Server{
	private:
		asio::io_context io_context_;
		tcp::acceptor acceptor_;
		std::thread io_context_thread;

		bool is_running = false;
		Server_context context;

	public:
		explicit Server();
		Server(const Server& other) = delete;
		Server operator=(const Server& other) = delete;
		~Server();

		void start(uint16_t port = 80);
		void stop();
		void stop(const std::string& message);

	private:
		void acceptor_init(uint16_t port);
		void start_accept();
	};
}

#endif//SERVER_H
