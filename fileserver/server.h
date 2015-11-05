#pragma once

#include <string>

namespace fserv {

class server {
public:
	server(const std::string &directory, int port);

	void start() const;

private:
	const std::string content_root;
	const int server_port;

};

}
