#include "net/io_context_wrapper.h"

#include <iostream>

net::Io_context_wrapper::Io_context_wrapper():
	io_context_ptr(nullptr){}
net::Io_context_wrapper::~Io_context_wrapper(){
	stop();
}

void net::Io_context_wrapper::init(){
	if(!io_context_ptr)
		io_context_ptr = std::make_unique<asio::io_context>();
}
asio::io_context& net::Io_context_wrapper::get()const{
	return *io_context_ptr;
}

void net::Io_context_wrapper::start(){
	thread = std::thread(&Io_context_wrapper::thread_func, this);
}
void net::Io_context_wrapper::thread_func(){
	if(io_context_ptr){
		io_context_ptr->run();
		std::cout << "io_context has completed its work" << std::endl;
	}
}
void net::Io_context_wrapper::stop(){
	if(io_context_ptr)
		io_context_ptr->stop();	
}
void net::Io_context_wrapper::reset(){
	stop();
	if(thread.joinable())
		thread.join();
	io_context_ptr.reset(nullptr);
}
