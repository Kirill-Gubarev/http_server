CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_NAME}
	src/
	external/asio/asio/include/
)
ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp

	src/def/default_definitions.h
	src/def/array_4KB.h

	src/net/server.cpp				src/net/server.h
	src/net/session.cpp				src/net/session.h
	src/net/session_manager.cpp		src/net/session_manager.h

	src/utils/command_line.cpp		src/utils/command_line.h
	src/utils/file_data.cpp			src/utils/file_data.h
	src/utils/thread_safe_set.hpp

	src/http/http_handler.cpp		src/http/http_handler.h
	src/http/http_utils.cpp			src/http/http_utils.h
	src/http/http_routing.cpp		src/http/http_routing.h

	src/html/html_render.cpp		src/html/html_render.h
)
