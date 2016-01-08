#include <iostream>
#include <memory>

#include "server.h"
#include "socket.h"

std::unique_ptr<core::node> sctl;

extern "C" {

void socket_init(core::engine &e) {
	std::cout << "socket init\n";
	sctl = std::make_unique<os::tcp_acceptor>(e, 8080);
}

void socket_uninit(core::engine &e) {
	std::cout << "socket uninit\n";
	sctl = nullptr;
}

}
