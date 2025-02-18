#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <cstdint>
#include <unordered_map>

namespace http{
	using std::string;

	enum class Http_method : uint8_t{
		UNKNOWN,
		GET,
		POST,
		PUT,
		DELETE,
	};

	struct Http_request{
		Http_method method;
		string url;
		std::unordered_map<string, string> fields;
		string body;

		explicit Http_request(string&& raw_data);

		void set(const string& key, const string& value);
		string get(const string& key) const;

		static Http_method string_to_method(const string& str);
	};
}

#endif//HTTP_REQUEST_H
