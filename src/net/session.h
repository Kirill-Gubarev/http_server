#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>
#include "def/array_4KB.h"

namespace net{
	class Session_manager;
	using asio::ip::tcp;

	class Session : public std::enable_shared_from_this<Session>{
	private:
		Session_manager& session_manager;
		tcp::socket socket_;
		array_4KB receive_buffer;
		std::string request;

	public:
		explicit Session(tcp::socket&& socket_, Session_manager& session_manager);
		Session(const Session& other) = delete;
		Session operator=(const Session& other) = delete;
		~Session();
		void close();

		void start();
		void send(const std::string& data);

	private:
		void send_limit_4KB(const char* const data, size_t length);

		void receive();
		void receive_callback(asio::error_code& ec, size_t length);
		void send_callback(asio::error_code& ec, size_t length);
	};
}
#endif//SESSION_H
