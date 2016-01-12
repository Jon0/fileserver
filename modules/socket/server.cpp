#include <iostream>

#include "server.h"

std::unique_ptr<core::node> sctl;

void socket_init(core::engine &e) {
	sctl = std::make_unique<sock::socketctl>(e);
}

void socket_uninit(core::engine &e) {
	sctl = nullptr;
}

namespace sock {


socketctl::socketctl(core::engine &e)
	:
	core::node(e, "socketctl") {
	acceptors.emplace_back(std::make_unique<tcp_acceptor>(e, 8080));
}


void socketctl::create_notify(core::node *other) {}


void socketctl::remove_notify(core::node *other) {}


void socketctl::recieve(core::channel &c, const core::object &obj) {}


void socketctl::update() {}


core::object socketctl::transform(const core::object &obj) const {}


}
