#include "net/session_manager.h"

#include "net/session.h"

net::Session_manager::Session_manager(core::Server_context& context):
	context(context){}
net::Session_manager::~Session_manager(){}

void net::Session_manager::create_session(tcp::socket&& socket_){
	auto [it, inserted] = session_map.emplace(next_id, std::make_unique<Session>(next_id++, std::move(socket_), context));
	if(inserted)
		it->second->start();
}
void net::Session_manager::delete_session(Session& session){
	session_map.erase(session.get_id());	
}
size_t net::Session_manager::size()const{
	return session_map.size();
}
void net::Session_manager::clear(){
	session_map.clear();
}
