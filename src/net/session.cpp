#include "session.h"

#include "net/server.h"
#include "http/http_handler.h"

#include <iostream>
#include <algorithm>
#include <string>

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
	socket_.async_read_some(asio::buffer(buffer, buffer.size()),
		[this](asio::error_code ec, size_t length){
			receive_callback(ec, length);
		}
	);
}
void net::Session::receive_callback(asio::error_code& ec, size_t length){
	if(!ec){
		request.append(buffer.data(), length);
		if(request.find("\r\n\r\n") != string::npos){
			context.http_handler.process_request(*this, std::move(request));
		}
		receive();
	}
	else if(ec == asio::error::eof){
		std::cout << "EOF" << std::endl;
		close();
	}
	else{
		std::cout << "receving error: " << ec.message() << "\n";
	}
}
void net::Session::send(const std::string& data){
	std::shared_ptr<string> data_ptr = std::make_shared<string>(data);
	send_data_in_chunks(data_ptr);
}
void net::Session::send_data_in_chunks(std::shared_ptr<string> data_ptr, size_t start){
	size_t length = std::min(data_ptr->length() - start, static_cast<size_t>(4*KB)); // <= 4KB
	if (length == 0) return;
    asio::async_write(socket_, asio::buffer(data_ptr->data() + start, length),
        [this, start, data_ptr](asio::error_code ec, size_t length){
			if (ec){
				std::cout << "sending error: " << ec.message() << std::endl;
				return;
			}
			std::cout << "sent " << length << " bytes." << std::endl;
			send_data_in_chunks(data_ptr, start + length);
        }
	);
}
