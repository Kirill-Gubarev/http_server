#include "session.h"

#include <iostream>
#include "core/http_core.h"
#include "server.h"

net::Session::Session(tcp::socket&& socket_, Server& server):
	socket_(std::move(socket_)), server(server){}

net::Session::~Session(){
	std::cout << "session has been deconstructed" << std::endl;
}
void net::Session::receive(){
	socket_.async_read_some(asio::buffer(receive_buffer, receive_buffer.size()),
		[this](asio::error_code ec, size_t lenght){
			if(!ec){
				request.append(receive_buffer.data(), receive_buffer.size());
				core::process_request(*this, std::move(request));
				receive();
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
void net::Session::send(const std::string message){
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
void net::Session::send(const std::array<char, 1024> message, size_t length){
    asio::async_write(socket_, asio::buffer(message, length),
        [this](asio::error_code ec, size_t length){
            if (ec) {
                std::cerr << "Error: " << ec.message() << std::endl;
            } else {
                std::cout << "Sent: " << length << " bytes." << std::endl;
            }
        }
	);
}

void net::Session::send(const std::string& header, std::ifstream& file){
	send(header);
	std::array<char, 1024> buffer;
	while (file) {
        file.read(buffer.data(), buffer.size()); 
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0)
			send(buffer, bytes_read);
    }
}
