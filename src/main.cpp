#include "core/server.h"
#include "utils/str_utils.hpp"

core::Server server;

int main(int argc, char* argv[]){
	if(argc < 2){
		server.start(80);
		return 0;
	}

	uint16_t port;
	if(utils::parse(argv[1], &port))
		server.start(port);
	else
		server.start(80);
	return 0;
}
