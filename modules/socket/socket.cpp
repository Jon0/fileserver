#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "socket.h"

namespace sock {


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
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

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


bool readyfd(int fd) {

	/* Watch fd to see when it has input. */
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

    /* Wait up to five seconds. */
	struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    int retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

	if (retval == -1) {
		perror("ERROR on select");
	}
    else if (retval) {
		/* FD_ISSET(0, &rfds) will be true. */
    	return true;
	}
	return false;
}


tcp_acceptor::tcp_acceptor(core::engine &e, int port)
	:
	core::node(e, "accept" + std::to_string(port)),
	sockfd(listen_sockfd(port)) {
}


tcp_acceptor::~tcp_acceptor() {
	close(sockfd);
}


int tcp_acceptor::acceptfd(sockaddr_in &cli_addr) const {
	socklen_t clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("ERROR on accept");
	}
	return newsockfd;
}


core::node *tcp_acceptor::match(const core::node &from, const std::string &type) {
	return nullptr;
}


void tcp_acceptor::recieve(core::channel &c, const core::object &obj) {

}


void tcp_acceptor::update() {
	//std::thread t(..., n.get_engine());
	//t.join();

	// accept requests
	if (readyfd(sockfd)) {
		sockaddr_in cli_addr;
		int newfd = acceptfd(cli_addr);
		if (newfd >= 0) {
			sockets.emplace_back(std::make_unique<tcp_socket>(get_engine(), newfd, cli_addr));
			tcp_socket *s = sockets.back().get();

			// find a recieving channel
			for (auto n : get_engine().node_search(*this, "binary")) {
				s->channel_open(n);
			}
		}
	}
}


tcp_socket::tcp_socket(core::engine &e, int fd, const sockaddr_in &cli_addr)
	:
	core::node(e, "sockettcp"),
	sockfd(fd),
	remote_open(true) {
	char strbuf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, strbuf, INET_ADDRSTRLEN);
	ipv4_str = strbuf;
	port_str = std::to_string(ntohs(cli_addr.sin_port));
}


tcp_socket::~tcp_socket() {
	close(sockfd);
}


int tcp_socket::fd() const {
	return sockfd;
}


bool tcp_socket::is_open() const {
	return sockfd >= 0 && remote_open;
}


core::node *tcp_socket::match(const core::node &from, const std::string &type) {
	return nullptr;
}


void tcp_socket::recieve(core::channel &c, const core::object &obj) {
	if (obj.type() == core::value_type::record_type) {
		std::string buffer = obj["data"].as_string();
		if (buffer.length() > 0) {
			int n = write(sockfd, buffer.c_str(), buffer.length());
			if (n < 0) {
				perror("ERROR on write");
			}
			else if (n == 0) {
				remote_open = false;
			}
		}
	}
}


void tcp_socket::update() {
	if (!is_open()) {
		std::cout << "updating closed socket\n";
		return;
	}
	if (readyfd(sockfd)) {
		std::cout << "read socket\n";

		// report raw data
		char buffer[1024];
		int n = read(sockfd, buffer, 1024);
		if (n < 0) {
			perror("ERROR on read");
		}
		else if (n == 0) {
			remote_open = false;
		}
		else if (n) {
			core::object::record data = {
				{"type", "binary"},
				{"ipv4", ipv4_str},
				{"port", port_str},
				{"data", std::string(buffer, n)}
			};
			send_all(data);
		}
	}
}


}
