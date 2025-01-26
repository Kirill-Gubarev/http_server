#include "session.h"

#include "net/server.h"
#include "http/http_handler.h"

#include <iostream>
#include <algorithm>

net::Session::Session(tcp::socket&& socket_, Server_context& context):
	socket_(std::move(socket_)), context(context){}

net::Session::~Session(){
	std::cout << "session has been deconstructed" << std::endl;
}
void net::Session::close(){
	context.session_manager.delete_session(*this);
}
void net::Session::start(){
	receive();
}
void net::Session::receive(){
	socket_.async_read_some(asio::buffer(receive_buffer, receive_buffer.size()),
		[this](asio::error_code ec, size_t length){
			receive_callback(ec, length);
		}
	);
}
void net::Session::receive_callback(asio::error_code& ec, size_t length){
	if(!ec){
		request.append(receive_buffer.data(), receive_buffer.size());
		context.http_handler.process_request(*this, std::move(request));
		receive();
	}
	else if(ec == asio::error::eof){
		std::cout << "connection has been closed" << std::endl;
		close();
	}
	else{
		std::cout << "receving error: " << ec.message() << "\n";
	}
}
void net::Session::send(const std::string& data){
	size_t length = data.length();
	int counter = 0;
	for(size_t ptr_offset = 0; ptr_offset < length; ptr_offset += 4*KB){
		std::cout << ++counter << '\n';
		send_limit_4KB(data.data() + ptr_offset, length - ptr_offset);
	}
}
void net::Session::send_limit_4KB(const char* const data, size_t length){
	length = std::min(length, static_cast<size_t>(4*KB));
    asio::async_write(socket_, asio::buffer(data, length),
        [this](asio::error_code ec, size_t length){
			send_callback(ec, length);
        }
	);
}

void net::Session::send_callback(asio::error_code& ec, size_t length){
	if (ec)
		std::cerr << "Error: " << ec.message() << std::endl;
	else
		std::cout << "Sent: " << length << " bytes." << std::endl;
}
