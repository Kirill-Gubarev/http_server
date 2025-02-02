#ifndef NETWORK_ENGINE_H
#define NETWORK_ENGINE_H

#include "core/server_context.h"

#include <asio.hpp>
#include <cstdint>
#include <thread>
#include <string>

namespace net{
	using std::string;
	using asio::ip::tcp;

	class Network_engine{
	private:
		core::Server_context& context;
		asio::io_context io_context_;
		tcp::acceptor acceptor_;

		std::thread io_context_thread;
		bool is_running;

	public:
		explicit Network_engine(core::Server_context& context);
		~Network_engine();
		Network_engine(const Network_engine& other) = delete;
		Network_engine& operator=(const Network_engine& other) = delete;

		void async_start(uint16_t port = 80);
		void stop();
		void stop(const string& message);

	private:
		void acceptor_init(uint16_t port);
		void start_async_accept();
	};
}

#endif//NETWORK_ENGINE_H
