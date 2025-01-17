#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include <array>

namespace net{
	class Server;
	using asio::ip::tcp;
	using array_char = std::array<char, 4096>;

	class Session : public std::enable_shared_from_this<Session>{
	private:
		Server& server;
		tcp::socket socket_;
		array_char receive_buffer;
		std::string request;

	public:
		explicit Session(tcp::socket&& socket_, Server& server);
		Session(const Session& other) = delete;
		Session operator=(const Session& other) = delete;
		~Session();

		void receive();
		void send(const std::string message);
		void send(const array_char message, size_t length);
	};
}
#endif//SESSION_H
