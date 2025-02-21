#include "core/server.h"

#include "def/memory_units.h"

#include <iostream>

core::Server::Server():
	command_line(context),
	network_engine(context),
	session_manager(context), 
	http_handler(context), 
	html_renderer(context),
	file_cacher(512*MB),
	context(*this){}

core::Server_context::Server_context(Server& server):
	command_line(server.command_line),
	network_engine(server.network_engine),
	session_manager(server.session_manager), 
	http_handler(server.http_handler), 
	html_renderer(server.html_renderer),
	file_cacher(server.file_cacher){}

core::Server::~Server(){
	stop();
}

void core::Server::start(uint16_t port){
	if(is_running) return;
	is_running = true;

	std::cout << "starting the server..." << std::endl;
	context.network_engine.async_start(port);
	context.command_line.start();
}
void core::Server::stop(){
	if(!is_running) return;
	is_running = false;

	std::cout << "stopping the server..." << std::endl;
	context.network_engine.stop();
	context.command_line.stop();
}

