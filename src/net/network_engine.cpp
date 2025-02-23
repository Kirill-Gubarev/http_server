#include "net/network_engine.h"

#include "net/session_manager.h"

#include <iostream>

net::Network_engine::Network_engine(core::Server_context& context):
	context(context), is_running(false){}

net::Network_engine::~Network_engine(){}

int net::Network_engine::start(uint16_t port){
	if(!is_running){
		is_running = true;
		std::cout << "starting the network_engine...\n";
	}

	io_context_wrapper.init();
	acceptor_init(port);
	io_context_wrapper.start();

	return 1;
}
int net::Network_engine::stop(){
	if(!is_running) return 0;
	is_running = false;
	std::cout << "stopping the network_engine...\n";
	
	io_context_wrapper.stop();
	context.session_manager.clear();
	acceptors.clear();
	io_context_wrapper.reset();
	return 1;
}
int net::Network_engine::restart(uint16_t port){
	return stop() && start(port);
}
int net::Network_engine::acceptor_init(uint16_t port){
	asio::error_code ec;
	std::unique_ptr<tcp::acceptor> acceptor_ptr = 
		std::make_unique<tcp::acceptor>(io_context_wrapper.get());
		
	acceptor_ptr->open(tcp::v4(), ec);
	if(ec){
		std::cout << "opening connection error: " << ec.message() << '\n';
		return 0;
	}

	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
	acceptor_ptr->bind(endpoint, ec);
	if(ec){
		std::cout << "binding connection error: " << ec.message() << '\n';
		return 0;
	}

	acceptor_ptr->listen(asio::socket_base::max_listen_connections, ec);
	if(ec){
		std::cout << "listening connection error: " << ec.message() << '\n';
		return 0;
	}

	start_async_accept(acceptor_ptr.get());
	acceptors.push_back(std::move(acceptor_ptr));
	std::cout << "server is listening on: " 
					  << endpoint.address().to_string() 
					  << ":" << endpoint.port() << std::endl;
	return 1;
}
void net::Network_engine::start_async_accept(tcp::acceptor* acceptor_ptr){
	acceptor_ptr->async_accept(
		[this, acceptor_ptr](const asio::error_code& ec, tcp::socket socket_){
			if(!ec){
				std::cout << "accepted new connection!\n";
				context.session_manager.create_session(std::move(socket_));
			}
			else{
				std::cout << "error accepting: " + ec.message() + '\n';
			}
			start_async_accept(acceptor_ptr);
		}
	);	
}
