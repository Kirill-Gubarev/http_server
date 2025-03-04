#include "net/session_manager.h"

#include <iostream>
#include "net/session.h"
#include "http/http_handler.h"
#include "http/http_request.h"

net::Session_manager::Session_manager(core::Server_context& context):
	context(context){}
net::Session_manager::~Session_manager(){}

void net::Session_manager::create_session(tcp::socket&& socket_){
	auto [it, inserted] = session_map.emplace(next_id, std::make_unique<Session>(next_id, std::move(socket_)));
	next_id++;
	if(inserted)
		start_receive(*it->second);
}
size_t net::Session_manager::delete_session(uint64_t id){
	return session_map.erase(id);
}
size_t net::Session_manager::size()const{
	return session_map.size();
}
void net::Session_manager::clear(){
	session_map.clear();
}

void net::Session_manager::start_receive(Session& session){
	reset_timer(session);
	session.socket_.async_read_some(asio::buffer(session.buffer, session.buffer.size()),
		[this, id = session.id](asio::error_code ec, size_t length){
			if(!ec){
				receive_callback(id, length);
			}
			else if(ec == asio::error::eof){
				std::cout << "EOF" << std::endl;
				delete_session(id);
			}
			else{
				std::cout << "receving error: " << ec.message() << "\n";
			}
		}
	);
}
void net::Session_manager::receive_callback(uint64_t id, size_t length){
	auto it = session_map.find(id);
	if(it == session_map.end())
		return;

	Session& session = *it->second;
	session.request.append(session.buffer.data(), length);
	if(session.request.find("\r\n\r\n") != string::npos || session.request.size() >= 256*KB){
		for(char ch : session.request){
			if(ch == '\n')
				std::cout << "\033[34m\\n\033[0m\n";
			else if(ch == '\r')
				std::cout << "\033[31m\\r\033[0m";
			else 
				std::cout << ch;
		}
		std::cout << std::endl;
		http::Http_request request(std::move(session.request));
		context.http_handler.process_request(session, std::move(request));
	}
	
	start_receive(session);
}
void net::Session_manager::send_ptr(Session& session, const string* data_ptr){
	start_send(session, data_ptr, 0);
}
void net::Session_manager::send_copy(Session& session, const string& data){
	start_send(session, std::make_shared<string>(data), 0);
}
template<typename PTR>
void net::Session_manager::start_send(Session& session, PTR data_ptr, size_t start){
	size_t length = std::min(data_ptr->length() - start, static_cast<size_t>(4*KB)); // <= 4KB
	if (length == 0)
		return;

    asio::async_write(session.socket_, asio::buffer(data_ptr->data() + start, length),
        [this, id = session.id, data_ptr, start](asio::error_code ec, size_t length){
			if (ec){
				std::cout << "sending error: " << ec.message() << std::endl;
				return;
			}
			std::cout << "sent " << length << " bytes." << std::endl;

			auto it = session_map.find(id);
			if(it != session_map.end())
				start_send(*it->second, data_ptr, start + length);
        }
	);
}

void net::Session_manager::reset_timer(Session& session){
    session.timer.expires_after(std::chrono::seconds(10));
    session.timer.async_wait([this, id = session.id](const asio::error_code& ec) {
        if(!ec && delete_session(id))
			std::cout << "timeout reached, session closed" << std::endl;
    });
}
