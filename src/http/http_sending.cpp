#include "http/http_sending.h"

#include "net/session.h"
#include "http/http_utils.h"
#include "http/http_routing.h"
#include "html/html_render.h"
#include "utils/file_data.h"

using namespace http;

static string create_http_request(int http_code, const utils::file_data& file){
	return 
		"HTTP/1.1 " + std::to_string(http_code) + ' ' + http::get_description(http_code) + "\r\n"
		"Content-Type: " + http::get_mime(file.extension) + "; charset=UTF-8\r\n"
		"Content-Length: " + std::to_string(file.content.size()) + "\r\n"
		"Connection: close\r\n\r\n";
}
void http::send_http_request(net::Session& session, int http_code, const string& request_path){
	std::error_code ec;
	string file_path = get_route_file_path(request_path, ec);
	if(ec){
		send_error_http_request(session, 404);
		return;
	}
	utils::file_data file;
	utils::read_file_full(&file, "assets/" + file_path, ec);

	if(ec){
		if(ec == std::errc::io_error)
			send_error_http_request(session, 404);
		else
			send_error_http_request(session, 500);
		return;
	}

	session.send(create_http_request(http_code, file));
	session.send(file.content);
}
void http::send_error_http_request(net::Session& session, int http_code){
	utils::file_data file;
	html::render_error_page(http_code, &file);
	session.send(create_http_request(http_code, file));
	session.send(file.content);
}
