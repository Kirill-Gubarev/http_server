CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(http_server)
FILE(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_NAME}
	src/
	external/asio/asio/include/
)
ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp
	src/server.cpp				src/server.h
	src/command_line.cpp		src/command_line.h
	src/session.cpp				src/session.h
	src/http_core.cpp			src/http_core.h
)
