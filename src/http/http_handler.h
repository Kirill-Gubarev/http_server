#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <string>

namespace net{class Session;}

namespace http{
	using std::string;

	class Http_handler{
	private:

	public:
		explicit Http_handler();
		Http_handler(const Http_handler& other) = delete;	
		Http_handler operator=(const Http_handler& other) = delete;	
		~Http_handler();

		void process_request(net::Session& session, string&& request);

	private:
		void print_request(const string& request);
	};
}

#endif//HTTP_HANDLER_H
