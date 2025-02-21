#ifndef NETWORK_ENGINE_H
#define NETWORK_ENGINE_H

#include "core/server_context.h"

#include <asio.hpp>
#include <cstdint>
#include <thread>
#include <string>
#include <memory>

namespace net{
	using std::string;
	using asio::ip::tcp;

	class Network_engine{
	private:
		std::unique_ptr<asio::io_context> io_context_ptr;
		std::unique_ptr<tcp::acceptor> acceptor_ptr;
		core::Server_context& context;
		std::thread io_context_thread;
		bool is_running;

	public:
		explicit Network_engine(core::Server_context& context);
		~Network_engine();
		Network_engine(const Network_engine& other) = delete;
		Network_engine& operator=(const Network_engine& other) = delete;

		int async_start(uint16_t port);
		int stop();
		int restart(uint16_t port);

	private:
		void start_async_accept();
		int acceptor_init(uint16_t port);
	};
}

#endif//NETWORK_ENGINE_H
