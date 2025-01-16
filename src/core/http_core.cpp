#include "http_core.h"
#include <iostream>
#include <fstream>

namespace core{

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
		std::ifstream file("assets/hello_world.html");
		if(file.is_open()){
			session.send(
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Connection: close\r\n\r\n",
				file
			);
			file.close();
		}else{
			
		}
	}
}
void process_request(net::Session& session, string&& request){
	print_request(request);
	handle_request(session, request);
}

}
