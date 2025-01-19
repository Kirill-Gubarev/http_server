#ifndef HTML_RENDER_H
#define HTML_RENDER_H

#include <string>
#include "utils/file_data.h"

namespace html{
	using std::string;

	void render_error_page(int http_code, utils::file_data* const file);	
}

#endif//HTML_RENDER_H
