#pragma once

#include <string>

namespace fserv {
namespace os {

void error(const char *msg);

int listen_sockfd(int port);

class tcp_acceptor {
public:
	tcp_acceptor(int port);
	~tcp_acceptor();

	int acceptfd() const;

private:
	const int sockfd;

};

class tcp_stream {
public:
	tcp_stream(const tcp_acceptor &a);
	~tcp_stream();

	std::string reads();
	void writes(const std::string &s);

private:
	const int sockfd;

};

}
}
