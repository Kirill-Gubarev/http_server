#ifndef STR_HANDLER_H
#define STR_HANDLER_H

#include <string_view>
#include <string>

namespace utils{
	using std::string_view;
	using std::string;

	class Str_handler{
	private:
		string_view str;

	public:
		class iterator{
		private:
			std::string_view str;
			std::string_view current_word;
			size_t pos;

		public:
			iterator(string_view str, size_t pos = 0);

			std::string_view operator*() const;
			iterator& operator++();
			bool operator!=(const iterator& other) const;
			size_t get_pos()const;

		private:
			void next();
		};

		Str_handler(string_view str);

		void skip_first_word();
		string get_first_word()const;
		string next_word();
		string_view str_view()const;

		iterator begin()const;
		iterator end()const;
	};
}

#endif//STR_HANDLER_H
