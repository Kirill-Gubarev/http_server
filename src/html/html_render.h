#ifndef HTML_RENDER_H
#define HTML_RENDER_H

#include <string>

namespace html{
	using std::string;

	string render_error_page(int http_code);	
}

#endif//HTML_RENDER_H
