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
	src/utils/thread_safe_set.hpp

	src/core/core.cpp				src/core/core.h
	src/core/http_core.cpp			src/core/http_core.h
	src/core/http_sending.cpp		src/core/http_sending.h
	src/core/html_render.cpp		src/core/html_render.h
)
