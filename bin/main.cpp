#include <iostream>

#include <core/engine.h>
#include <common/server.h>

int main(int argc, char *argv[]) {
	core::engine e;
	e.start();

	std::string directory = ".";
	int port = 8080;

	for (int i = 1; i < argc - 1; ++i) {
		std::string arg = argv[i];
		if (arg == "-d") {
			directory = argv[i + 1];
		}
		else if (arg == "-p") {
			port = atoi(argv[i + 1]);
		}
	}

	// use the main libraries
	std::cout << "serving " << directory << " on port " << port << "\n";
	fserv::server fileserver(directory, port);
	fileserver.start();
	return 0;
}
