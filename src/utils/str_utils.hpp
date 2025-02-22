#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <string>
#include <string_view>
#include <charconv>
#include <cstring>

namespace utils{
	using std::string;
	using std::string_view;

	template <typename NUM> int parse(const char* str, NUM* num);
	template <typename NUM> int parse(const string& str, NUM* num);
	template <typename NUM> int parse(const string_view& str, NUM* num);
}

template <typename NUM> int utils::parse(const char* str, NUM* num){
	auto [ptr, ec] = std::from_chars(str, str + strlen(str), *num);
	if(ec == std::errc())
		return 1;
	return 0;
}
template <typename NUM> int utils::parse(const string& str, NUM* num){
	auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), *num);
	if(ec == std::errc())
		return 1;
	return 0;
}
template <typename NUM> int utils::parse(const string_view& str, NUM* num){
	auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), *num);
	if(ec == std::errc())
		return 1;
	return 0;
}

#endif//STR_UTILS_H
