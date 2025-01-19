#include "html/html_render.h"

#include "utils/file_utils.h"
#include "http/http_utils.h"

namespace html{

static const string default_error_page = R"(<!DOCTYPE html>
<head>
    <title>{{ERROR}}</title>
</head>
<body>
    <h1>{{ERROR}}</h1>
    <a href="/">Go back to homepage</a>
</body>)";

static void replace_placeholders(string* const page, const string& placeholder, const string& value){
	size_t pos = 0;
	while((pos = page->find(placeholder, pos)) != string::npos){
		page->replace(pos, placeholder.size(), value);
		pos += value.size();
	}
}

string render_error_page(int http_code){
	std::string file = utils::read_file_full("assets/templates/error_page.html");
	if(file.size() == 0)
		file = default_error_page;
	replace_placeholders(
			&file, "{{ERROR}}", 
			std::to_string(http_code) + ' ' + http::get_description(http_code));
	return file;
}

}// namespace html
