#include <memory>
#include <thread>

#include "socket.h"
#include "server.h"

std::unique_ptr<core::node> sctl;

void init(core::engine &e) {
	sctl = std::make_unique<os::tcp_acceptor>(e, 8080);
	e.node_open(sctl.get());
}

void uninit(core::engine &e) {
	e.node_close(sctl.get());
	sctl = nullptr;
}
