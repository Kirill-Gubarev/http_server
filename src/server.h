#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <cstdint>
#include <thread>

namespace net{
using asio::ip::tcp;

class Server{
private:
	asio::io_context io_context_;
	tcp::resolver resolver_;
	std::thread io_context_thread;
	bool is_running = false;

public:
	explicit Server(uint16_t port = 80);
	Server(const Server& other) = delete;
	Server operator=(const Server& other) = delete;
	~Server();

	void start();
	void stop();
};
}

#endif//SERVER_H
