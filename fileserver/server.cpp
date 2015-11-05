#include "http/request.h"
#include "http/response.h"
#include "io/strings.h"
#include "os/file.h"
#include "os/pipe.h"
#include "os/socket.h"

#include "server.h"

namespace fserv {

server::server(const std::string &directory, int port)
	:
	content_root(directory),
	server_port(port) {
}

void server::start() const {

	// accept http requests
	os::tcp_acceptor webport(server_port);
	while (true) {
		os::tcp_stream stream(webport);
		http::request r = http::parse_request("tcp", stream.reads());
		std::vector<std::string> url = io::split(r.location, '/');
		
		os::directory dir(content_root);
		std::string result;
		for (auto &s : dir.file_list()) {
			result += s + "\n";
		}

		stream.writes(http::create_response(result));
	}
}

}
