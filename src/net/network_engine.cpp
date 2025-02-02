#include "net/network_engine.h"

#include "net/session_manager.h"

#include <iostream>

net::Network_engine::Network_engine(core::Server_context& context):
	context(context), acceptor_(io_context_), is_running(false){}
net::Network_engine::~Network_engine(){}

void net::Network_engine::async_start(uint16_t port){
	if(is_running) return;
	is_running = true;

	acceptor_init(port);	
	start_async_accept();

	io_context_thread = std::thread([this](){
		io_context_.run();
		std::cout << "io_context has completed its work" << std::endl;
	});
	io_context_thread.detach();
}
void net::Network_engine::stop(){
	if(!is_running) return;
	is_running = false;

	io_context_.stop();
}
void net::Network_engine::stop(const string& message){
	std::cout << message << std::endl;
	stop();
}
void net::Network_engine::acceptor_init(uint16_t port){
	asio::error_code ec;
		
	acceptor_.open(tcp::v4(), ec);
	if(ec) stop("opening connection error: " + ec.message());

	asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
    acceptor_.bind(endpoint, ec);
	if(ec) stop("binding connection error: " + ec.message());

    acceptor_.listen(asio::socket_base::max_listen_connections, ec);
	if(ec) stop("listening connection error: " + ec.message());

	asio::ip::tcp::endpoint local_endpoint = acceptor_.local_endpoint();
	std::cout << "server is listening on: " 
					  << local_endpoint.address().to_string() 
					  << ":" << local_endpoint.port() << std::endl;
}
void net::Network_engine::start_async_accept(){
	acceptor_.async_accept(
		[this](const asio::error_code& ec, tcp::socket socket_){
			if(!ec){
				std::cout << "accepted new connection!\n";
				context.session_manager.create_session(std::move(socket_));
			}
			else{
				std::cout << "error accepting: " + ec.message() + '\n';
			}
			start_async_accept();
		}
	);	
}
