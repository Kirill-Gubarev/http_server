#include "http/http_request.h"

#include <sstream>

void http::Http_request::set(const string& key, const string& value){
	fields[key] = value;
}
std::string http::Http_request::get(const string& key) const{
	auto it = fields.find(key);
	if(it != fields.end())
		return it->second;

	return "";
}

http::Http_request::Http_request(string&& raw_data){
	size_t body_pos = raw_data.find("\r\n\r\n");
	if (body_pos == string::npos){
		method = Http_method::UNKNOWN;
		return;	
	}
	body_pos += 4; // size of \r\n\r\n

	std::istringstream iss(std::move(raw_data));
	string line;

	if (getline(iss, line)) {
		std::istringstream line_iss(line);
		string method_str, url;
		line_iss >> method_str >> url;

		this->method = string_to_method(method_str);
		this->url = std::move(url);
	}
	while(getline(iss, line)){
		size_t pos = line.find(": ");
		if (pos != string::npos) {
			string key = line.substr(0, pos);
			string value = line.substr(pos + 2);
			set(key, value);
		}
		else{
			break;
		}
	}
	std::string body((std::istreambuf_iterator<char>(iss)), std::istreambuf_iterator<char>());
	this->body = std::move(body);
}

http::Http_method http::Http_request::string_to_method(const string& str){
	if (str == "GET") return Http_method::GET;
	if (str == "POST") return Http_method::POST;
	if (str == "PUT") return Http_method::PUT;
	if (str == "DELETE") return Http_method::DELETE;
	return Http_method::UNKNOWN;	
}
