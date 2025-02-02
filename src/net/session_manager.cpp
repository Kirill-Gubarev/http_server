#include "net/session_manager.h"

#include "net/session.h"

net::Session_manager::Session_manager(core::Server_context& context):
	context(context){}
net::Session_manager::~Session_manager(){}

void net::Session_manager::create_session(tcp::socket&& socket_){
	std::shared_ptr<Session> session_ptr = std::make_shared<Session>(std::move(socket_), context);
	session_set.insert(session_ptr);
	session_ptr->start();
}
void net::Session_manager::delete_session(Session& session){
	session_set.erase(session.shared_from_this());	
}
