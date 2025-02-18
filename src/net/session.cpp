#include "session.h"

#include "session_manager.h"

net::Session::Session(uint64_t id, tcp::socket&& socket_):
	id(id), socket_(std::move(socket_)), timer(socket_.get_executor()){}
