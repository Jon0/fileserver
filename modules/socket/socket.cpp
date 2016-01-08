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


void tcp_acceptor::recieve(core::channel &remote, const core::object &obj) {

}

void tcp_acceptor::update() {
	//std::thread t(..., n.get_engine());
	//t.join();

	// accept http requests
	while (true) {
		std::cout << "wait for connection\n";
		os::tcp_socket *s = new os::tcp_socket(get_engine(), *this);
		get_engine().node_open(s);
	}
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


void tcp_socket::recieve(core::channel &remote, const core::object &obj) {

}


void tcp_socket::update() {
	// report raw data
	char buffer[1024];
	int n = read(sockfd, buffer, 1024);

	// add a core::object (uses shared_ptr anyway)
	core::object data_obj(std::string(buffer, n));

	// copy to all connected channels (that are owned by this)
	for (core::channel *c : owned_channels()) {
		c->send(data_obj);
	}
}


}
