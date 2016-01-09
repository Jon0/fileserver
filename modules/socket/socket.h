#pragma once

#include <string>
#include <netinet/in.h>
#include <core/engine.h>

namespace sock {

class tcp_socket;

void error(const char *msg);

int listen_sockfd(int port);

bool readyfd(int fd);

class tcp_acceptor : public core::node {
public:
	tcp_acceptor(core::engine &e, int port);
	~tcp_acceptor();

	int acceptfd(sockaddr_in &cli_addr) const;

	core::node *match(const core::node &from, const std::string &type) override;
	void recieve(core::channel &c, const core::object &obj) override;
	void update() override;

private:
	const int sockfd;
	std::vector<std::unique_ptr<tcp_socket>> sockets;

};

class tcp_socket : public core::node {
public:
	tcp_socket(core::engine &e, int fd, const sockaddr_in &cli_addr);
	~tcp_socket();

	int fd() const;
	bool is_open() const;

	core::node *match(const core::node &from, const std::string &type) override;
	void recieve(core::channel &c, const core::object &obj) override;
	void update() override;

private:
	const int sockfd;
	std::string ipv4_str;
	std::string port_str;
	bool remote_open;

};

}
