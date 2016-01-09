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


core::node *socketctl::match(const core::node &from, const std::string &type) {
	return nullptr;
}


void socketctl::recieve(core::channel &c, const core::object &obj) {

}


void socketctl::update() {

}


}
