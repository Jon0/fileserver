#include <algorithm>
#include <iostream>

#include "channel.h"
#include "engine.h"

namespace core {


node::node(node_set &e, const std::string &name)
    :
    ns(e),
    name(name) {
    ns.node_open(this);
}


node::~node() {
    ns.node_close(this);
}

node_set &node::get_set() {
    return ns;
}


std::string node::get_name() const {
    return name;
}


int node::channel_count() const {
    return channels.size();
}


channel *node::channel_open(const std::string &type, node *other) {
    std::cout << "[" << name << "] connecting to " << other->get_name() << " (" << type << ")\n";
    channels.emplace_back(std::make_unique<channel>(type, *this));
    channel *opened_channel = channels.back().get();
    opened_channel->set_reciever(other);
    return opened_channel;
}


void node::channel_close(node *other) {
    std::cout << "[" << name << "] disconnect " << other->get_name() << "\n";
    channels.erase(
        std::remove_if(
            channels.begin(),
            channels.end(),
            [other](std::unique_ptr<channel> &c) {
                return c->get_reciever() == other;
            }),
        channels.end()
    );
    reply_channels.erase(
        std::remove_if(
            reply_channels.begin(),
            reply_channels.end(),
            [other](channel *c) {
                return &c->owner == other;
            }),
        reply_channels.end()
    );
}


void node::channel_copy_all(node *other) {
    std::cout << "[" << name << "] copying channels from " << other->get_name() << "\n";
    for (channel *c : other->owned_channels()) {
        channel_open(c->get_type(), c->get_reciever());
    }
}


void node::register_channel(channel *c) {
    reply_channels.push_back(c);
}


void node::deregister_channel(channel *c) {
    reply_channels.erase(
        std::remove_if(
            reply_channels.begin(),
            reply_channels.end(),
            [c](channel *other) {
                return c == other;
            }),
        reply_channels.end()
    );
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
	for (auto &c : channels) {
		c->send(obj);
	}
}


void node::reply_all(const object &obj) {

    // copy to all connected channels (that are owned by this)
    for (channel *c : reply_channels) {
        c->reply(obj);
    }
}


void node::msg(channel &c, const object &obj) {
    std::cout << "[" << name << "] recieve " << c.get_type() << "\n";
    recieve(c, obj);
}


void node::event(const std::string &type, const object &obj) {
    // pass event to required channels
    // events only trigger owned channels?
    bool data_sent = false;
    for (auto &c : channels) {
        if (c->get_type() == type) {
            c->send(obj);
            data_sent = true;
        }
    }

    if (!data_sent) {
        std::cout << "[" << name << "] " << type << " event was ignored\n";
        for (auto &c : channels) {
            std::cout << "[" << name << "] " << c->get_type() << "\n";
        }
    }
}

void node::reply(const std::string &name, const std::string &type, const object &obj) {
    std::cout << "there are " << reply_channels.size() << " return channels\n";
    for (channel *c : reply_channels) {
        if (c->get_name() == name && c->get_type() == type) {
            c->reply(obj);
        }
    }
}


channel::channel(const std::string &type, node &owner)
    :
    owner(owner),
    content_type(type) {
}


void channel::send(const object &obj) {
    if (remote) {
        remote->msg(*this, obj);
    }
}


void channel::reply(const object &obj) {
    owner.msg(*this, obj);
}


bool channel::is_reply(node *other) const {
    return other == &owner;
}


std::string channel::get_name() const {

    // channel name matches the owner
    return owner.get_name();
}


std::string channel::get_type() const {
    return content_type;
}


node *channel::get_reciever() const {
    return remote;
}


void channel::set_reciever(node *other) {
    if (other) {
        other->register_channel(this);
        remote = other;
    }
}


}
