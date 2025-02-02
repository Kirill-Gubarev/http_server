#include "html/html_renderer.h"

#include "http/http_utils.h"
#include "file/file_data.h"
#include "file/file_cacher.h"

static const std::string default_error_page = R"(<!DOCTYPE html>
<head>
    <title>{{ERROR}}</title>
</head>
<body>
    <h1>{{ERROR}}</h1>
    <a href="/">Go back to homepage</a>
</body>)";

html::Html_renderer::Html_renderer(core::Server_context& context): context(context){}

void html::Html_renderer::render_error_page(int http_code, file::File_data* page_ptr){
	const file::File_data* error_page_ptr = context.file_cacher.get_file_ptr("assets/templates/error_page.html");

	if(!error_page_ptr)
		page_ptr->content = default_error_page;
	else
		page_ptr->content = error_page_ptr->content;

	replace_placeholders(
			&page_ptr->content, "{{ERROR}}", 
			std::to_string(http_code) + ' ' + http::get_description(http_code));
}
void html::Html_renderer::replace_placeholders(string* page_ptr, const string& placeholder, const string& value){
	size_t pos = 0;
	while((pos = page_ptr->find(placeholder, pos)) != string::npos){
		page_ptr->replace(pos, placeholder.size(), value);
		pos += value.size();
	}
}
