#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/server_context.h"

#include <unordered_map>
#include <asio.hpp>
#include <memory>

namespace net{
	using asio::ip::tcp;

	class Session_manager{
	private:
		core::Server_context& context;
		std::unordered_map<uint64_t, std::unique_ptr<Session>> session_map;
		uint64_t next_id;

	public:
		explicit Session_manager(core::Server_context& context);	
		Session_manager(const Session_manager& other) = delete;	
		Session_manager operator=(const Session_manager& other) = delete;	
		~Session_manager();	
		
		void create_session(tcp::socket&& socket_);
		void delete_session(Session& session);

		size_t size()const;
		void clear();
	};
}

#endif//SESSION_MANAGER_H
