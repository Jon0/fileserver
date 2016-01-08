#include <iostream>

#include "channel.h"
#include "engine.h"

namespace core {


node::node(engine &e, const std::string &name)
    :
    engi(e),
    name(name) {
    engi.node_open(this);
}


node::~node() {
    engi.node_close(this);
}

engine &node::get_engine() {
    return engi;
}


std::string node::get_name() const {
    return name;
}


void node::channel_open(node *other) {
    std::cout << "[" << name << "] connecting to " << other->get_name() << "\n";

    channels.emplace_back(std::make_unique<channel>(*this));
    channels.back()->set_reciever(other);
}


std::vector<channel *> node::owned_channels() const {
    std::vector<channel *> result;
    for (auto &c : channels) {
        result.emplace_back(c.get());
    }
    return result;
}


void node::send_all(const object &obj) {

    // copy to all connected channels (that are owned by this)
	for (channel *c : owned_channels()) {
		c->send(obj);
	}
}


void node::msg(channel &c, const object &obj) {
    std::cout<< "[" << name << "] " << obj.str() << "\n";
    recieve(c, obj);
}


channel::channel(node &owner)
    :
    owner(owner),
    name("unnamed") {
}


void channel::send(const object &obj) {
    if (remote) {
        remote->msg(*this, obj);
    }
}


void channel::reply(const object &obj) {
    owner.msg(*this, obj);
}

void channel::set_reciever(node *other) {
    remote = other;
}


}
