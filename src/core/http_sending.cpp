#include "http_sending.h"

#include "net/session.h"
#include "core/core.h"

#include <array>
#include <fstream>
#include <iostream>

using array_char = std::array<char, 4096>;
using std::string;

namespace core{
	void send(net::Session& session, const string& message){
		session.send(message);
	}
	void send_file_chunked(net::Session& session, std::ifstream& file){
		array_char buffer;
		while (file) {
			file.read(buffer.data(), buffer.size()); 
			std::streamsize bytes_read = file.gcount();
			if (bytes_read > 0)
				session.send(buffer, bytes_read);
		}
	}
	void send_file_full(net::Session& session, std::ifstream& file){
		std::ostringstream buffer;
		buffer << file.rdbuf();
		session.send(buffer.str());
	}
}


string core::create_http_request(int http_code, const string& mime, size_t body_size){
	return
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + get_description(http_code) + "\r\n"
		"Content-Type: " + mime + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(body_size) + "\r\n"
		"Connection: close\r\n\r\n";
}
void core::send_http_request(net::Session& session, int http_code, const string& file_path){
	std::ifstream file("assets/" + file_path, std::ios::binary | std::ios::ate);
	if(file.is_open()){
		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);
		send(
			session,
			create_http_request(http_code, get_mime_file_type(file_path), file_size));
		send_file_full(session, file);
		file.close();
	}
	else{
		std::cout << "\033[31mfile opening error\033[0m\n";	
		if(http_code != 500)
			send_http_request(session, 500, "internal_server_error.html");	
	}
}
