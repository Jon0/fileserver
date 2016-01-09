#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

namespace os {


void error(const char *msg) {
	perror(msg);
	exit(1);
}


int listen_sockfd(int port) {
	// open socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	// bind and listen on socket
	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}
	listen(sockfd, 5);
	return sockfd;
}


tcp_acceptor::tcp_acceptor(core::engine &e, int port)
	:
	core::node(e, "socketctl"),
	connected(false),
	sockfd(listen_sockfd(port)) {
}


tcp_acceptor::~tcp_acceptor() {
	close(sockfd);
}


int tcp_acceptor::acceptfd() const {
	sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	return newsockfd;
}


core::node *tcp_acceptor::match(const core::node &from, const std::string &type) {
	return nullptr;
}


void tcp_acceptor::recieve(core::channel &c, const core::object &obj) {

}


void tcp_acceptor::update() {
	if (connected) {
		return;
	}
	//std::thread t(..., n.get_engine());
	//t.join();

	// accept http requests
	std::cout << "wait for connection\n";
	os::tcp_socket *s = new os::tcp_socket(get_engine(), *this);

	// find a recieving channel
	for (auto n : get_engine().node_search(*this, "binary")) {
		s->channel_open(n);
	}
	connected = true;
}


tcp_socket::tcp_socket(core::engine &e, const tcp_acceptor &a)
	:
	core::node(e, "sockettcp"),
	sockfd(a.acceptfd()) {
}


tcp_socket::~tcp_socket() {
	close(sockfd);
}


int tcp_socket::fd() const {
	return sockfd;
}


core::node *tcp_socket::match(const core::node &from, const std::string &type) {
	return nullptr;
}


void tcp_socket::recieve(core::channel &c, const core::object &obj) {
	if (obj.type() == core::value_type::record_type) {
		std::string buffer = obj["data"].as_string();
		if (buffer.length() > 0) {
			int n = write(sockfd, buffer.c_str(), buffer.length());
		}
	}
}


void tcp_socket::update() {
	// report raw data
	char buffer[1024];
	int n = read(sockfd, buffer, 1024);

	core::object::record data = {
		{"type", "binary"},
		{"port", "8080"},
		{"data", std::string(buffer, n)}
	};

	send_all(data);
}


}
