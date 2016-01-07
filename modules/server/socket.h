#pragma once

#include <string>
#include <core/engine.h>

namespace os {

void error(const char *msg);

int listen_sockfd(int port);

class tcp_acceptor : public core::channel {
public:
	tcp_acceptor(core::engine &e, int port);
	~tcp_acceptor();

	int acceptfd() const;

	void msg_module(const core::object &obj) override;
	void update() override;

private:
	const int sockfd;

};

class tcp_socket : public core::channel {
public:
	tcp_socket(core::engine &e, const tcp_acceptor &a);
	~tcp_socket();

	int fd() const;

	void msg_module(const core::object &obj) override;
	void update() override;

private:
	const int sockfd;


};

}
