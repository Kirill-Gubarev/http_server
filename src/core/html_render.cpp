#include "html_render.h"

#include "core/core.h"

using std::string;

static const string default_error_page = R"(<!DOCTYPE html>
<head>
    <title>500 Internal Server Error</title>
</head>
<body>
    <h1>Error 500 Internal Server Error</h1>
    <a href="/">Go back to homepage</a>
</body>)";

static void replace_placeholders(string* page, const string& placeholder, const string& value){
	size_t pos = 0;
	while((pos = page->find(placeholder, pos)) != string::npos){
		page->replace(pos, placeholder.size(), value);
		pos += value.size();
	}
}

string core::render_error_page(int code){
	std::string file = read_file("assets/templates/error_page.html");
	if(file.size() == 0)
		return default_error_page;
	replace_placeholders(&file, "{{ERROR}}", std::to_string(code) + ' ' + get_description(code));
	return file;
}
