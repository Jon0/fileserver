#pragma once

#include <iostream>
#include <string>

#include "http/request.h"
#include "os/file.h"

namespace fserv {


class server {
public:
	server(const std::string &directory, int port);

	void start() const;

	std::string file_ext(const std::string &s) const;

	os::location path_of(const std::string request_path) const;

private:
	void handle_get(std::iostream &stream, const http::request &r) const;

	std::string return_file(const os::location &loc) const;

	const std::string content_root;
	const int server_port;

};


}
