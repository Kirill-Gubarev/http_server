#include "http/http_sending.h"

#include "net/session.h"
#include "http/http_utils.h"
#include "http/http_routing.h"
#include "html/html_render.h"

#include <array>
#include <fstream>
#include <iostream>

using namespace http;
using array_char = std::array<char, 4096>;

void http::send_message(net::Session& session, const string& message){
	session.send(message);
}
void http::send_file_chunked(net::Session& session, const string& file_path){
	std::ifstream file(file_path, std::ios::binary);
	if(!file.is_open()){
		std::cout << "\033[31mfile opening error: " + file_path + "\033[0m\n";	
		return;
	}

	array_char buffer;
	while (file) {
		file.read(buffer.data(), buffer.size()); 
		std::streamsize bytes_read = file.gcount();
		if (bytes_read > 0)
			session.send(buffer, bytes_read);
	}
}
void http::send_file_full(net::Session& session, const utils::file_data& file){
	session.send(file.content);
}

static string create_http_request(int http_code, const utils::file_data& file){
	return 
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n"
		"Content-Type: " + http::get_mime(file.extension) + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(file.content.size()) + "\r\n"
		"Connection: close\r\n\r\n";
}
void http::send_http_request(net::Session& session, int http_code, const string& request_path){
	std::error_code ec;
	utils::file_data file;
	string file_path = get_route_file_path(request_path, ec);
	if(ec){
		send_error_http_request(session, 404);
		return;
	}

	utils::read_file_full(&file, "assets/" + file_path, ec);

	if(ec){
		if(ec == std::errc::io_error)
			send_error_http_request(session, 404);
		else
			send_error_http_request(session, 500);
		return;
	}

	send_message(session, create_http_request(http_code, file));
	send_file_full(session, file);
}
void http::send_error_http_request(net::Session& session, int http_code){
	utils::file_data file;
	html::render_error_page(http_code, &file);
	send_message(session, create_http_request(http_code, file));
	send_file_full(session, file);
}
