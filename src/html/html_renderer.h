#ifndef HTML_RENDERER_H
#define HTML_RENDERER_H

#include "file/file_data.h"
#include "core/server_context.h"

#include <string>

namespace html{
	using std::string;

	class Html_renderer{
	private:
		core::Server_context& context;

	public:
		explicit Html_renderer(core::Server_context& context);
		void render_error_page(int http_code, file::File_data* file_ptr);	

	private:
		void replace_placeholders(
				string* page, 
				const string& placeholder, 
				const string& value);
	};
}

#endif//HTML_RENDERER_H
