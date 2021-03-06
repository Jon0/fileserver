#include <iostream>

#include "route.h"

std::unique_ptr<core::node> rctl;

void route_init(core::engine &e) {}

void route_uninit(core::engine &e) {}

namespace route {


routectl::routectl(core::node_set &e)
    :
    core::node(e, "routectl") {
}


void routectl::handle_get(const core::object &obj) {
    std::string url = obj["url"].as_string();
    std::cout << "http request for " << url << " recieved\n";
    std::string ss = url.substr(1);
    node *matched_node = get_set().node_get(ss);

    // get status of other nodes
    if (matched_node) {
        core::object::record data = {
            {"node", obj["node"]},
            {"type", "info"},
            {"info", enable_info},
            {"count", request_count++}
        };
        reply("httpctl", "http", data);
    }
    else {
        core::object::record data = {
            {"node", obj["node"]},
            {"type", "path"},
            {"source", obj},
            {"path", url},
            {"action", "open"},
        };
        event("route", data);
    }
}


void routectl::create_notify(core::node *other) {}


void routectl::remove_notify(core::node *other) {}


void routectl::recieve(core::channel &c, const core::object &obj) {
    if (obj["type"].as_string() == "http") {
        handle_get(obj);
    }
}

void routectl::update() {}

core::object routectl::transform(const core::object &obj) const {

}

}
