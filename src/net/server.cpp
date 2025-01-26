#include "server.h"

#include <iostream>
#include "session.h"

net::Server::Server():
	io_context_(), acceptor_(io_context_), context(){}
net::Server_context::Server_context():
	session_manager(*this){}

net::Server::~Server(){
	stop();
}

void net::Server::start(uint16_t port){
	if(is_running) return;
	is_running = true;

	std::cout << "starting the server..." << std::endl;

	acceptor_init(port);
	start_accept();

	io_context_thread = std::thread([this](){
		io_context_.run();
		std::cout << "io_context has completed its work" << std::endl;
	});
	io_context_thread.detach();
}
void net::Server::stop(){
	if(!is_running) return;
	is_running = false;

	io_context_.stop();

	std::cout << "stopping the server..." << std::endl;
}
void net::Server::stop(const std::string& message){
	std::cout << message << std::endl;
	stop();
}

void net::Server::acceptor_init(uint16_t port){
	asio::error_code ec;
		
	acceptor_.open(tcp::v4(), ec);
	if(ec) stop("opening connection error: " + ec.message());

    acceptor_.bind(tcp::endpoint(tcp::v4(), port), ec);
	if(ec) stop("binding connection error: " + ec.message());

    acceptor_.listen(asio::socket_base::max_listen_connections, ec);
	if(ec) stop("listening connection error: " + ec.message());
}
void net::Server::start_accept(){
	acceptor_.async_accept(
		[this](const asio::error_code& ec, tcp::socket socket_){
			if(!ec){
				std::cout << "accepted new connection!\n";
				context.session_manager.create_session(std::move(socket_));
			}
			else{
				std::cout << "error accepting: " + ec.message() + '\n';
			}
			start_accept();
		}
	);	
}
