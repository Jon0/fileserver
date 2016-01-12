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
        std::make_tuple("socketctl", "httpctl", "tcp"),
        std::make_tuple("accept8080", "httpctl", "tcp"),
        std::make_tuple("httpctl", "routectl", "http"),
        std::make_tuple("routectl", "filectl", "route"),
        std::make_tuple("routectl", "httpctl", "status"),
        std::make_tuple("filectl", "templatectl", "dir"),
        std::make_tuple("filectl", "templatectl", "file"),
        std::make_tuple("filectl", "httpctl", "chr"),
        std::make_tuple("templatectl", "httpctl", "http")
    };
}


void flowctl::create_notify(core::node *other) {
    name_match(other->get_name(), other);
}


void flowctl::remove_notify(core::node *other) {
    connected.erase(other->get_name());
}


void flowctl::recieve(core::channel &c, const core::object &obj) {

}


void flowctl::update() {

}


core::object flowctl::transform(const core::object &obj) const {

}


void flowctl::name_match(const std::string &name, core::node *n) {
    for (auto &l : links) {
        if (std::get<0>(l) == name && connected.count(std::get<1>(l)) > 0) {
            make_link(n, get_engine().node_get(std::get<1>(l)), std::get<2>(l));
        }
        else if (std::get<1>(l) == name && connected.count(std::get<0>(l)) > 0) {
            make_link(get_engine().node_get(std::get<0>(l)), n, std::get<2>(l));
        }
    }
    connected.insert(name);
}


void flowctl::make_link(core::node *from, core::node *to, const std::string &type) {
    if (from && to) {
        from->channel_open(type, to);
    }
}


}
