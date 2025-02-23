#ifndef IO_CONTEXT_WRAPPER_H
#define IO_CONTEXT_WRAPPER_H

#include <asio.hpp>
#include <thread>
#include <memory>

namespace net{
	class Io_context_wrapper{
	private:
		std::unique_ptr<asio::io_context> io_context_ptr;
		std::thread thread;

	public:
		Io_context_wrapper();			
		Io_context_wrapper(const Io_context_wrapper& other) = delete;			
		Io_context_wrapper operator=(const Io_context_wrapper& other) = delete;			
		~Io_context_wrapper();			

		void init();
		asio::io_context& get()const;

		void start();
		void stop();
		void reset();

	private:
		void thread_func();
	};
}

#endif//IO_CONTEXT_WRAPPER_H
