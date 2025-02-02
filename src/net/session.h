#ifndef SESSION_H
#define SESSION_H

#include "def/memory_units.h"
#include "core/server_context.h"

#include <asio.hpp>
#include <array>

namespace net{
	using asio::ip::tcp;
	using std::string;

	class Session : public std::enable_shared_from_this<Session>{
	private:
		core::Server_context& context;
		tcp::socket socket_;
		std::array<char, 4*KB> buffer;
		std::string request;

	public:
		explicit Session(tcp::socket&& socket_, core::Server_context& context);
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
