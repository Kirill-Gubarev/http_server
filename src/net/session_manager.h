#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <unordered_set>
#include <asio.hpp>
#include <memory>

namespace net{
	using asio::ip::tcp;
	class Server_context;
	class Session;

	class Session_manager{
	private:
		std::unordered_set<std::shared_ptr<Session>> session_set;
		Server_context& context;

	public:
		explicit Session_manager(Server_context& context);	
		Session_manager(const Session_manager& other) = delete;	
		Session_manager operator=(const Session_manager& other) = delete;	
		~Session_manager();	
		
		void create_session(tcp::socket&& socket_);
		void delete_session(Session& session);
	private:
		
	};
}

#endif//SESSION_MANAGER_H
