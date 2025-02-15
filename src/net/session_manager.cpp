#include "net/session_manager.h"

#include <iostream>
#include "net/session.h"
#include "http/http_handler.h"

net::Session_manager::Session_manager(core::Server_context& context):
	context(context){}
net::Session_manager::~Session_manager(){}

void net::Session_manager::create_session(tcp::socket&& socket_){
	auto [it, inserted] = session_map.emplace(next_id, std::make_unique<Session>(next_id, std::move(socket_)));
	next_id++;
	if(inserted)
		start_receive(*it->second);
}
void net::Session_manager::delete_session(uint64_t id){
	session_map.erase(id);
}
size_t net::Session_manager::size()const{
	return session_map.size();
}
void net::Session_manager::clear(){
	session_map.clear();
}

void net::Session_manager::start_receive(Session& session){
	session.socket_.async_read_some(asio::buffer(session.buffer, session.buffer.size()),
		[this, id = session.id](asio::error_code ec, size_t length){
			receive_callback(id, ec, length);
		}
	);
}
void net::Session_manager::receive_callback(uint64_t id, asio::error_code& ec, size_t length){
	if(!ec){
		auto it = session_map.find(id);
		if(it == session_map.end())
			return;

		Session& session = *it->second;
		session.request.append(session.buffer.data(), length);
		if(session.request.find("\r\n\r\n") != string::npos){
			context.http_handler.process_request(session, std::move(session.request));
		}
		start_receive(session);
	}
	else if(ec == asio::error::eof){
		std::cout << "EOF" << std::endl;
		delete_session(id);
	}
	else{
		std::cout << "receving error: " << ec.message() << "\n";
	}
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
