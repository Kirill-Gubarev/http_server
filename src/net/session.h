#ifndef SESSION_H
#define SESSION_H

#include "def/memory_units.h"

#include <asio.hpp>
#include <array>
#include <cstdint>

namespace net{
	using asio::ip::tcp;
	using std::string;

	struct Session{
		const uint64_t id;
		tcp::socket socket_;
		std::array<char, 4*KB> buffer;
		std::string request;
		asio::steady_timer timer;

		explicit Session(uint64_t id, tcp::socket&& socket_);
		Session(const Session& other) = delete;
		Session operator=(const Session& other) = delete;
		~Session();
	};
}
#endif//SESSION_H
