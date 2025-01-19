CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(http_server)
FILE(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_NAME}
	src/
	external/asio/asio/include/
)
ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp

	src/net/server.cpp				src/net/server.h
	src/net/session.cpp				src/net/session.h

	src/utils/command_line.cpp		src/utils/command_line.h
	src/utils/file_utils.cpp		src/utils/file_utils.h
	src/utils/thread_safe_set.hpp

	src/http/http_core.cpp			src/http/http_core.h
	src/http/http_sending.cpp		src/http/http_sending.h
	src/http/http_utils.cpp			src/http/http_utils.h

	src/html/html_render.cpp		src/html/html_render.h
)
