#include "net/server.h"
#include "utils/command_line.h"

net::Server server;
utils::Command_line command_line(server);

int main(){
	server.start();
	command_line.run();
	return 0;
}
