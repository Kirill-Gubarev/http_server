#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/server_context.h"

#include <unordered_map>
#include <asio.hpp>
#include <memory>
#include <string>

namespace net{
	using asio::ip::tcp;
	using std::string;

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
		size_t delete_session(uint64_t id);
		size_t size()const;
		void clear();

		void send_ptr(Session& session, const string* data_ptr);
		void send_copy(Session& session, const string& data);

	private:
		void start_receive(Session& session);
		void receive_callback(uint64_t id , size_t length);
		
		template<typename PTR>
		void start_send(Session& session, PTR data_ptr, size_t start);

		void reset_timer(Session& session);
	};
}

#endif//SESSION_MANAGER_H
