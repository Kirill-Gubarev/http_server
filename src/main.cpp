#include "core/server.h"
#include "utils/str_utils.hpp"

core::Server server;

int main(int argc, char* argv[]){
	uint16_t port;
	if(argc > 1 && utils::parse(argv[1], &port))
		server.run(port);
	else
		server.run(80);
	return 0;
}
