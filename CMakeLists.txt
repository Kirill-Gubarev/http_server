CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
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

	src/def/memory_units.h

	src/core/server.cpp				src/core/server.h
									src/core/server_context.h

	src/ctrl/command_line.cpp		src/ctrl/command_line.h
	src/ctrl/commands.cpp

	src/net/network_engine.cpp		src/net/network_engine.h
	src/net/session_manager.cpp		src/net/session_manager.h
	src/net/session.cpp				src/net/session.h

	src/file/file_cacher.cpp		src/file/file_cacher.h
	src/file/file_data.cpp			src/file/file_data.h

	src/http/http_handler.cpp		src/http/http_handler.h
	src/http/http_request.cpp		src/http/http_request.h
	src/http/http_utils.cpp			src/http/http_utils.h
	src/http/http_routing.cpp		src/http/http_routing.h

	src/html/html_renderer.cpp		src/html/html_renderer.h

	src/utils/str_handler.cpp		src/utils/str_handler.h
	src/utils/str_iterator.cpp
	src/utils/str_utils.hpp
)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
