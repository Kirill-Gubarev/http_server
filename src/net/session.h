#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include <array>
#include <fstream>

namespace net{
	class Server;
	using asio::ip::tcp;

	class Session : public std::enable_shared_from_this<Session>{
	private:
		Server& server;
		tcp::socket socket_;
		std::array<char, 1024> receive_buffer;
		std::string request;

	public:
		explicit Session(tcp::socket&& socket_, Server& server);
		Session(const Session& other) = delete;
		Session operator=(const Session& other) = delete;
		~Session();

		void receive();
		void send(const std::string message);
		void send(const std::array<char, 1024> message, size_t length);
		void send(const std::string& header, std::ifstream& body);
	};
}
#endif//SESSION_H
