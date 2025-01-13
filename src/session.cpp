#include "session.h"
#include <iostream>
#include "http_core.h"

net::Session::Session(tcp::socket&& socket_):
	socket_(std::move(socket_)), request_buffer(){}

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
			else if(ec == asio::error::eof)
				std::cout << "connection has been closed" << std::endl;
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
