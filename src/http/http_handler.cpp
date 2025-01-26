#include "http/http_handler.h"

#include "http_sending.h"

#include <iostream>
#include <sstream>

http::Http_handler::Http_handler(){}
http::Http_handler::~Http_handler(){}

void http::Http_handler::print_request(const string& request){
	for(char ch : request){
		if(ch == '\n')
			std::cout << "\033[34m\\n\033[0m\n";
		else if(ch == '\r')
			std::cout << "\033[31m\\r\033[0m";
		else 
			std::cout << ch;
	}
}

void http::Http_handler::process_request(net::Session& session, string&& request){
	print_request(request);

	std::istringstream iss(request);
	string method, request_path;
	iss >> method >> request_path;
	if(method == "GET")
		send_http_request(session, 200, request_path);
	else
		send_error_http_request(session, 405);
}
