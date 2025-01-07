#include "server.h"
#include <iostream>
#include <chrono>

net::Server::Server(uint16_t port):
	io_context_(), resolver_(io_context_){}

net::Server::~Server(){
	stop();
}

void net::Server::start(){
	if(is_running) return;
	is_running = true;

	std::cout << "starting the server..." << std::endl;
	io_context_thread = std::thread([this](){
			while(is_running){
				std::cout << "the server is working...\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(700));
			}
		});
	io_context_thread.detach();
}
void net::Server::stop(){
	if(!is_running) return;
	is_running = false;

	std::cout << "stopping the server..." << std::endl;
}
