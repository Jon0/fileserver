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
	tcp_acceptor(core::node_set &e, int port);
	~tcp_acceptor();

	int acceptfd(sockaddr_in &cli_addr) const;

	void create_notify(core::node *other) override;
	void remove_notify(core::node *other) override;
	void recieve(core::channel &c, const core::object &obj) override;
	void update() override;
	core::object transform(const core::object &obj) const override;

private:
	const int sockfd;
	std::vector<std::unique_ptr<tcp_socket>> sockets;
	int next_sock_id;

};

class tcp_socket : public core::node {
public:
	tcp_socket(core::node_set &e, int fd, const std::string &name, const sockaddr_in &cli_addr);
	~tcp_socket();

	int fd() const;
	bool is_open() const;

	void create_notify(core::node *other) override;
	void remove_notify(core::node *other) override;
	void recieve(core::channel &c, const core::object &obj) override;
	void update() override;
	core::object transform(const core::object &obj) const override;

private:
	const int sockfd;
	std::string ipv4_str;
	std::string port_str;
	bool remote_open;

};

}
