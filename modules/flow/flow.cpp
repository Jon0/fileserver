#include <iostream>

#include "flow.h"

std::unique_ptr<core::node> fctl;

void flow_init(core::engine &e) {
    fctl = std::make_unique<flow::flowctl>(e);
}

void flow_uninit(core::engine &e) {
    fctl = nullptr;
}

namespace flow {


flowctl::flowctl(core::engine &e)
    :
    core::node(e, "flowctl") {
    links = {
        std::make_tuple("httpctl", "routectl", "http"),
        std::make_tuple("routectl", "filectl", "route"),
        std::make_tuple("routectl", "httpctl", "status")
    };
}


void flowctl::create_notify(core::node *other) {
    std::string other_name = other->get_name();

    node *http = get_engine().node_get("httpctl");
    if (http) {
        std::cout << "create tcp channel\n";
        other->channel_open("tcp", http);
    }

    for (auto &l : links) {
        if (std::get<0>(l) == other_name) {
            make_link(other, get_engine().node_get(std::get<1>(l)), std::get<2>(l));
        }
        else if (std::get<1>(l) == other_name) {
            make_link(get_engine().node_get(std::get<0>(l)), other, std::get<2>(l));
        }
    }
}


void flowctl::remove_notify(core::node *other) {

}


void flowctl::recieve(core::channel &c, const core::object &obj) {

}


void flowctl::update() {

}


void flowctl::make_link(core::node *from, core::node *to, const std::string &type) {
    if (from && to) {
        std::cout << "create " << type << " channel to " << to->get_name() << "\n";
        from->channel_open(type, to);
    }
}


}
