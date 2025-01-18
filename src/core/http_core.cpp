#include "http_core.h"

#include "net/session.h"
#include "core.h"
#include "core/http_sending.h"

#include <iostream>

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

void handle_request(net::Session& session, const string& request){
	std::istringstream iss(request);
	string word;
	iss >> word;
	if(word == "GET"){
		iss >> word;
		if(word == "/")
			send_http_request(session, 200, "hello_world.html");
		else if(word == "/test")
			send_http_request(session, 200, "test.html");
		else if(word == "/clicker")
			send_http_request(session, 200, "clicker.html");
		else if(word == "/clicker.js")
			send_http_request(session, 200, "clicker.js");
		else
			send_http_request(session, 400, "bad_request.html");
	}
	else{
		send_http_request(session, 501, "bad_request.html");
	}
}
void process_request(net::Session& session, string&& request){
	print_request(request);
	handle_request(session, request);
}

}
