#include "session.h"

#include <iostream>
#include "http_core.h"
#include "server.h"

net::Session::Session(tcp::socket&& socket_, Server& server):
	socket_(std::move(socket_)), server(server){}

net::Session::~Session(){
	std::cout << "session has been deconstructed" << std::endl;
}
void net::Session::read(){
	socket_.async_read_some(asio::buffer(request_buffer, request_buffer.size()),
		[this](asio::error_code ec, size_t lenght){
			if(!ec){
				request.append(request_buffer.data(), request_buffer.size());
				core::process_request(*this, std::move(request));
				read();
			}
			else if(ec == asio::error::eof){
				std::cout << "connection has been closed" << std::endl;
				server.close_session(*this);
			}
			else
				std::cout << "receving error: " << ec.message() << "\n";
		}
	);
}
void net::Session::write(const std::string& message){
    asio::async_write(socket_, asio::buffer(message),
        [this](asio::error_code ec, size_t length){
            if (ec) {
                std::cerr << "Error: " << ec.message() << std::endl;
            } else {
                std::cout << "Sent: " << length << " bytes." << std::endl;
            }
        }
	);
}
