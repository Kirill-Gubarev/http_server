#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H

namespace ctrl{class Command_line;}
namespace net{class Network_engine; class Session_manager; class Session;}
namespace http{class Http_handler;}
namespace html{class Html_renderer;}
namespace file{class File_cacher;}

namespace core{
	class Server;
	struct Server_context{
		ctrl::Command_line& command_line;
		net::Network_engine& network_engine;
		net::Session_manager& session_manager;		
		http::Http_handler& http_handler;
		html::Html_renderer& html_renderer;
		file::File_cacher& file_cacher;

		explicit Server_context(Server& server);
		Server_context(const Server_context& other) = delete;
		Server_context operator=(const Server_context& other) = delete;
	};
}

#endif//SERVER_CONTEXT_H
