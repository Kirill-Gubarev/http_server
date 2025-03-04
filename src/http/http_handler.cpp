#include "http/http_handler.h"

#include "http/http_request.h"
#include "net/session.h"
#include "net/session_manager.h"
#include "file/file_cacher.h"
#include "http/http_utils.h"
#include "http/http_routing.h"
#include "html/html_renderer.h"
#include "file/file_data.h"

#include <iostream>

http::Http_handler::Http_handler(core::Server_context& context): context(context){}
http::Http_handler::~Http_handler(){}


void http::Http_handler::process_request(net::Session& session, Http_request&& request){
	switch(request.method){
		case http::Http_method::GET:
			send_http_request(session, 200, request.url);
		break;
		case http::Http_method::POST:
			std::cout << "\033[32m" << request.body << "\033[0m\n";
		break;
		default:
			send_error_http_request(session, 405);
	}
}

static std::string create_http_request(int http_code, const file::File_data& file){
	return 
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n"
		"Content-Type: " + http::get_mime(file.extension) + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(file.content.size()) + "\r\n"
		"Connection: close\r\n\r\n";
}
void http::Http_handler::send_http_request(net::Session& session, int http_code, const string& url){
	string request_path = "";
	if(!url.empty())
		request_path = url.substr(1);
	string file_path;

	const file::File_data* file_ptr;
	if(get_route_file_path(request_path, &file_path)){
		file_ptr = context.file_cacher.get_file_ptr("assets/" + file_path);
	}
	else{
		file_path = "assets/" + request_path;
		if(!is_allowed_path(file_path)){
			send_error_http_request(session, 403);	
			return;
		}
		file_ptr = context.file_cacher.get_file_ptr(file_path);
		if(!file_ptr){
			file_path += ".html";
			if(!is_allowed_path(file_path)){
				send_error_http_request(session, 403);	
				return;
			}
			file_ptr = context.file_cacher.get_file_ptr(file_path);
		}
	}

	if(!file_ptr){
		send_error_http_request(session, 404);
		return;
	}

	context.session_manager.send_copy(session, create_http_request(http_code, *file_ptr));
	context.session_manager.send_ptr(session, &file_ptr->content);
}
void http::Http_handler::send_error_http_request(net::Session& session, int http_code){
	file::File_data file;
	context.html_renderer.render_error_page(http_code, &file);
	context.session_manager.send_copy(session, create_http_request(http_code, file));
	context.session_manager.send_copy(session, file.content);
}
bool http::Http_handler::is_allowed_path(const string& path) const{
	return path.find("..") == string::npos && 
		path.find("~") == string::npos &&
		path.find("//") == string::npos;
}
