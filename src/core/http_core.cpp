#include "http_core.h"

#include "core.h"
#include <iostream>
#include <fstream>
#include "net/session.h"

namespace core{
	using array_char = std::array<char, 4096>;

void print_request(const string& request){
	for(char ch : request){
		if(ch == '\n')
			std::cout << "\033[34m\\n\033[0m\n";
		else if(ch == '\r')
			std::cout << "\033[31m\\r\033[0m";
		else 
			std::cout << ch;
	}
}

void inline send_header(net::Session& session, const string& header){
	session.send(header);
}
void send_file(net::Session& session, std::ifstream& file){
	array_char buffer;
	while (file) {
        file.read(buffer.data(), buffer.size()); 
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0)
			session.send(buffer, bytes_read);
    }
}

void send_file(net::Session& session, int http_code, const string& path){
	std::ifstream file("assets/" + path, std::ios::binary | std::ios::ate);
	if(file.is_open()){
		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);
		send_header(
			session,
			"HTTP/1.1 " + std::to_string(http_code) + ' ' + get_description(http_code) + "\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: " + std::to_string(file_size) + "\r\n"
			"Connection: close\r\n\r\n");
		send_file(session, file);
		file.close();
	}
	else{
		std::cout << "\033[31mfile opening error\033[0m\n";	
		if(http_code != 500)
			send_file(session, 500, "internal_server_error.html");	
	}
}
void handle_request(net::Session& session, const string& request){
	std::istringstream iss(request);
	string word;
	iss >> word;
	if(word == "GET"){
		iss >> word;
		if(word == "/")
			send_file(session, 200, "hello_world.html");
		else if(word == "/test")
			send_file(session, 200, "test.html");
		else
			send_file(session, 400, "bad_request.html");
	}
	else{
		send_file(session, 501, "bad_request.html");
	}
}
void process_request(net::Session& session, string&& request){
	print_request(request);
	handle_request(session, request);
}

}
