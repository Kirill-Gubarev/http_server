#include "session.h"

#include "session_manager.h"

#include <iostream>

net::Session::Session(uint64_t id, tcp::socket&& socket_):
	id(id), socket_(std::move(socket_)), timer(socket_.get_executor()){}

net::Session::~Session(){
	std::cout << "session has been deconstructed" << std::endl;
}
