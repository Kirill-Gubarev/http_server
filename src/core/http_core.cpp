#include "http_core.h"
#include <iostream>

void core::process_request(net::Session& session, const string& request){
	std::cout << request << std::endl;
	session.write(
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n"
		"Connection: close\r\n\r\n"
		"<!DOCTYPE html>"
		"<html>"
		"<head><title>Title</title></head>"
		"<body><h1>Hello world!</h1></body>"
		"</html>"
	);
}
