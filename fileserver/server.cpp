#include <chrono>
#include <fstream>

#include "http/response.h"
#include "io/strings.h"
#include "io/template.h"
#include "os/pipe.h"
#include "os/socket.h"

#include "server.h"

namespace fserv {

constexpr int buf_size = 4 * 1024;

server::server(const std::string &directory, int port)
	:
	content_root(directory),
	server_port(port) {
}


void server::start() const {

	// accept http requests
	os::tcp_acceptor webport(server_port);
	while (true) {
		os::tcp_socket socket(webport);
		os::fdbuf buffer(socket.fd());
		std::iostream stream(&buffer);

		// parse http request
		http::request r = http::read_request(stream);
		std::cout << r.info() << "\n";

		switch (r.data.type) {
		case http::request_type::http_get:
			handle_get(stream, r);
			break;
		default:

			// handle other request types
			stream << "HTTP/1.1 501 Not Implemented\n\n";
		}
	}
}


os::location server::path_of(const std::string request_path) const {
	return os::location(content_root + request_path);
}


void server::handle_get(std::iostream &stream, const http::request &r) const {

	//
	// find the requested location
	std::string result;
	os::location loc = path_of(r.data.location);
	if (!loc.exists()) {
		stream << "HTTP/1.1 404 Not Found\n\n";
	}
	else if (loc.isdir()) {
		result += "<h1>Index of " + r.data.location + "</h1>";
		result += "<table>";
		result += "<tr>";
		result += "<th valign=\"top\"></th><th><a>Name</a></th><th><a>Last modified</a></th><th><a>Size</a></th><th><a>Description</a></th>";
		result += "</tr>";
		result += "<tr><th colspan=\"5\"><hr></th></tr>";


		os::directory dir(loc.path());
		for (auto &s : dir.file_list()) {
			os::location f = loc.append(s);
			std::string link = r.data.location;
			if (link.back() != '/') {
				link += "/" + s;
			}
			else {
				link += s;
			}
			result += "<tr>";
			result += "<td valign=\"top\"></td><td><a href="+ link +">" + s + "</a></td>";
			result += "<td align=\"right\">" + f.modified() + "</td>";
			result += "<td align=\"right\">" + f.sizestr() + "</td>";
				result += "<td align=\"right\">" + f.mode() + "</td>";
				result += "</tr>";
		}


		result += "<tr><th colspan=\"5\"><hr></th></tr>";
		result += "</table>";

		// return page
		stream << http::create_response(result);
	}
	else {
		return_file(stream, loc);
	}
}


std::string server::file_ext(const std::string &s) const {
	return s.substr(s.find('.', 1) + 1);
}


void server::return_file(std::iostream &stream, const os::location &loc) const {
	if (loc.isexec()) {

		// execute script
		// todo set environment vars
		std::string result = os::exec(loc.path());
		stream << http::create_response(result);
	}
	else if (loc.ischr()) {

		// stream a file
		stream << http::basic_header(200, "text/html");
		std::cout << "device " << loc.path() << " requested\n";
		std::ifstream src_stream(loc.path(), std::ios::in | std::ios::binary);

		// transfer rate
		int transferred = 0;
		auto start_time = std::chrono::system_clock::now();
		char buffer[buf_size];
		while (true) {

			// read from device
			src_stream.read(buffer, buf_size);
			stream << std::string(buffer, buf_size);

			// record transfer rate
			transferred += buf_size;
			if (transferred > (1 << 16)) {
				auto end_time = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end_time - start_time;
				auto rate = static_cast<double>(transferred) / (elapsed_seconds.count() * 1024 * 1024);
				transferred = 0;
				start_time = end_time;
				std::cout << "transferring " << rate << " Mbps\n";
			}
		}
	}
	else {

		// switch based on file type
		std::string type = file_ext(loc.path());
		std::cout << "filetype: " << type << "\n";

		// enable templating
		std::string file_content = io::read_file(loc.path());
		io::tmpl t(file_content);
		std::string result = t.render([](const std::string &in) {
			return os::exec(in);
		});

		stream << http::create_response(result);
	}
}


}
