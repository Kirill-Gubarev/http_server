#include "http/http_handler.h"

#include "http/http_request.h"
#include "net/session.h"
#include "net/session_manager.h"
#include "file/file_cacher.h"
#include "http/http_utils.h"
#include "http/http_routing.h"
#include "html/html_renderer.h"
#include "file/file_data.h"
#include "db/db_manager.h"

#include <nlohmann/json.hpp>
#include <iostream>

http::Http_handler::Http_handler(core::Server_context& context): context(context){}
http::Http_handler::~Http_handler(){}

static std::string create_http_request(int http_code, const file::File_data& file){
	return 
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n"
		"Content-Type: " + http::get_mime(file.extension) + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(file.content.size()) + "\r\n"
		"Connection: close\r\n\r\n";
}
static std::string create_http_request_json(int http_code, const std::string& json) {
    return 
        "HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n" + 
        "Content-Type: application/json; charset=UTF-8\r\n" + 
        "Content-Length: " + std::to_string(json.size()) + "\r\n" + 
        "Access-Control-Allow-Origin: *\r\n" + 
        "Access-Control-Allow-Methods: GET\r\n" + 
        "Access-Control-Allow-Headers: Content-Type\r\n" + 
        "Cache-Control: no-cache\r\n" + 
        "Connection: close\r\n\r\n" + 
        json;
}

void http::Http_handler::post(net::Session& session, Http_request& request)const{
	try{
		nlohmann::json json = nlohmann::json::parse(request.body);
		string action = json["action"];
		if(action == "login"){
			context.session_manager.send_copy(session,
				create_http_request_json(
					200, 
					context.db_manager.get_user(json["login"], json["password"])
					));
		}
		else if(action == "registration"){
			if(context.db_manager.create_user(
						json["login"], json["password"],json["email"],json["fullname"], 5000)){
			context.session_manager.send_copy(session, create_http_request_json(200, 
					context.db_manager.get_user(json["login"], json["password"])
					));
			}
			else{
				context.session_manager.send_copy(session,
					create_http_request_json(200, "{\"success\":false}"));
			}
		}
		else if(action == "add_cart"){
			if(context.db_manager.add_product_to_cart(json["login"], json["product"], json["quantity"].get<int>())){
				context.session_manager.send_copy(session,
					create_http_request_json(200, "{\"success\":true}"));
			}
			else{
				context.session_manager.send_copy(session,
					create_http_request_json(200, "{\"success\":false}"));
			}
		}
		else if(action == "remove_cart"){
			if(context.db_manager.remove_from_cart(json["login"], json["product"])){
				context.session_manager.send_copy(session,
					create_http_request_json(200, "{\"success\":true}"));
			}
			else{
				context.session_manager.send_copy(session,
					create_http_request_json(200, "{\"success\":false}"));
			}
		}
		else if(action == "get_cart"){
			context.session_manager.send_copy(session, create_http_request_json(200, context.db_manager.get_cart_products(json["login"])));
		}
	}
	catch(std::exception& ex){
		std::cerr << ex.what() << std::endl;
	}
	context.session_manager.delete_session(session.id);
}
void http::Http_handler::process_request(net::Session& session, Http_request&& request){
	switch(request.method){
		case http::Http_method::GET:
			send_http_request(session, 200, request.url);
		break;
		case http::Http_method::POST:
			post(session, request);
		break;
		default:
			send_error_http_request(session, 405);
	}
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
	else if(request_path == "fruits"){
		context.session_manager.send_copy(session,
				create_http_request_json(
					200, context.db_manager.get_fruits()
					));
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
