#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include <array>

namespace net{

using asio::ip::tcp;

class Session{
private:
	tcp::socket socket_;

	std::array<char, 1024> request_buffer;
	std::string request;

public:
	explicit Session(tcp::socket&& socket_);
	Session(const Session& other) = delete;
	Session operator=(const Session& other) = delete;
	~Session();

	void read();
	void write(const std::string& message);
};
}
#endif//SESSION_H
