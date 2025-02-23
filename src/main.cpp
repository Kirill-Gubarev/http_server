#include "core/server.h"

core::Server server;

int main(int argc, char* argv[]){
	server.run(argv, argc);
	return 0;
}
