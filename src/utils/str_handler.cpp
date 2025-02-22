#include "utils/str_handler.h"

utils::Str_handler::Str_handler(string_view str): str(str){}
utils::Str_handler::iterator utils::Str_handler::begin()const{
	return iterator(str);
}
utils::Str_handler::iterator utils::Str_handler::end()const{
	return iterator(str, string_view::npos);
}
void utils::Str_handler::skip_first_word(){
	size_t pos = begin().get_pos();	
	if(pos != string_view::npos)
		str = str.substr(pos);	
	else
		str = "";
}
std::string utils::Str_handler::get_first_word()const{
	return string(*begin());
}
std::string utils::Str_handler::next_word(){
	string next_word = get_first_word();
	skip_first_word();
	return next_word;
}
std::string_view utils::Str_handler::str_view()const{
	return str;
}
