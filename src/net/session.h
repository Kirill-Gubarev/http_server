#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>

#include "def/array_4KB.h"

namespace net{
	class Server_context;
	using asio::ip::tcp;
	using std::string;

	class Session : public std::enable_shared_from_this<Session>{
	private:
		Server_context& context;
		tcp::socket socket_;
		array_4KB buffer;
		std::string request;

	public:
		explicit Session(tcp::socket&& socket_, Server_context& context);
		Session(const Session& other) = delete;
		Session operator=(const Session& other) = delete;
		~Session();
		void close();

		void start();
		void send(const std::string& data);

	private:
		void send_data_in_chunks(std::shared_ptr<string> data_ptr, size_t start = 0);

		void receive();
		void receive_callback(asio::error_code& ec, size_t length);
	};
}
#endif//SESSION_H
