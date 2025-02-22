#include "str_handler.h"

utils::Str_handler::iterator::iterator(string_view str, size_t pos):
	str(str), pos(pos){
		next();
}

void utils::Str_handler::iterator::next(){
	if (pos >= str.size()) {
		pos = string_view::npos;
		return;
	}

	size_t start = str.find_first_not_of(' ', pos);
	if (start == std::string_view::npos) {
		pos = string_view::npos;
		return;
	}

	size_t end = str.find(' ', start);
	current_word = str.substr(start, end - start);

	if(end == std::string_view::npos)
		pos = str.size();
	else
		pos = end + 1;
}

std::string_view utils::Str_handler::iterator::operator*()const{
	return current_word;
}

utils::Str_handler::iterator& utils::Str_handler::iterator::operator++(){
	next();
	return *this;
}
bool utils::Str_handler::iterator::operator!=(const utils::Str_handler::iterator& other) const{
	return pos != other.pos; 
}
size_t utils::Str_handler::iterator::get_pos()const{
	return pos;
}
