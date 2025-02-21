#include "core/server.h"

#include <cstdint>
#include <charconv>

core::Server server;

int main(int argc, char* argv[]){
	if(argc < 2){
		server.start(80);
		return 0;
	}

	uint16_t port;
	auto [ptr, ec] = std::from_chars(argv[1], argv[1] + strlen(argv[1]), port);
	if(ec == std::errc())
		server.start(port);
	else
		server.start(80);
	return 0;
}
