#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <cstdint>
#include <thread>
#include <vector>
#include "session.h"

namespace net{
using asio::ip::tcp;

class Server{
private:
	asio::io_context io_context_;
	tcp::acceptor acceptor_;
	std::thread io_context_thread;
	bool is_running = false;
	std::vector<std::unique_ptr<Session>> sessions;

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
