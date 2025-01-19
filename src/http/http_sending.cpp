#include "http/http_sending.h"

#include "net/session.h"
#include "utils/file_utils.h"
#include "http/http_utils.h"
#include "html/html_render.h"

#include <array>
#include <fstream>
#include <iostream>

namespace http{
	using array_char = std::array<char, 4096>;

void send_message(net::Session& session, const string& message){
	session.send(message);
}
void send_file_chunked(net::Session& session, string file_path){
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
void send_file_full(net::Session& session, const string& file){
	session.send(file);
}

void send_http_request(net::Session& session, int http_code, const string& file_path){
	string file = utils::read_file_full("assets/" + file_path);
	if(file.size() == 0){
		http_code = 500;
		file = html::render_error_page(http_code);
	}

	send_message(
		session,
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n"
		"Content-Type: " + http::get_mime(file_path) + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(file.size()) + "\r\n"
		"Connection: close\r\n\r\n");
	send_file_full(session, file);
}

}// namespace http
