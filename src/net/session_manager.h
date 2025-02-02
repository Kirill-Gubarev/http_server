#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/server_context.h"

#include <unordered_set>
#include <asio.hpp>
#include <memory>

namespace net{
	using asio::ip::tcp;

	class Session_manager{
	private:
		std::unordered_set<std::shared_ptr<Session>> session_set;
		core::Server_context& context;

	public:
		explicit Session_manager(core::Server_context& context);	
		Session_manager(const Session_manager& other) = delete;	
		Session_manager operator=(const Session_manager& other) = delete;	
		~Session_manager();	
		
		void create_session(tcp::socket&& socket_);
		void delete_session(Session& session);
	private:
		
	};
}

#endif//SESSION_MANAGER_H
