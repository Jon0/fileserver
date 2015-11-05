#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

namespace fserv {
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
	
tcp_acceptor::tcp_acceptor(int port)
	:
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

tcp_stream::tcp_stream(const tcp_acceptor &a)
	:
	sockfd(a.acceptfd()) {
}

tcp_stream::~tcp_stream() {
	close(sockfd);
}

std::string tcp_stream::reads() {
	std::string result = "";
	char buffer [256];
	int n;
	do {
		n = read(sockfd, buffer, 256);
		if (n < 0) {
			error("ERROR reading from socket");
		}
		result += std::string(buffer, n);
	} while (n == 256);
	return result;
}

void tcp_stream::writes(const std::string &s) {
	int out_n = write(sockfd, s.c_str(), s.length());
	if (out_n < 0) {
		error("ERROR writing to socket");
	}
}


}
}
