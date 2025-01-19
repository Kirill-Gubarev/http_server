#include "http/http_core.h"

#include "net/session.h"
#include "http/http_sending.h"

#include <iostream>

namespace http{

static void print_request(const string& request){
	for(char ch : request){
		if(ch == '\n')
			std::cout << "\033[34m\\n\033[0m\n";
		else if(ch == '\r')
			std::cout << "\033[31m\\r\033[0m";
		else 
			std::cout << ch;
	}
}

static void handle_request(net::Session& session, const string& request){
	std::istringstream iss(request);
	string method, request_path;
	iss >> method >> request_path;
	if(method == "GET")
		send_http_request(session, 200, request_path);
	else
		send_error_http_request(session, 405);
}
void process_request(net::Session& session, string&& request){
	print_request(request);
	handle_request(session, request);
}

}// namespace http
