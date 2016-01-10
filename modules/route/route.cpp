#include <iostream>

#include "route.h"

std::unique_ptr<core::node> rctl;

void route_init(core::engine &e) {
    rctl = std::make_unique<route::routectl>(e);
}

void route_uninit(core::engine &e) {
    rctl = nullptr;
}

namespace route {


routectl::routectl(core::engine &e)
    :
    core::node(e, "routectl") {
}


void routectl::handle_get(const core::object &obj) {
    std::string url = obj["url"].as_string();
    std::cout << "http request for " << url << " recieved\n";
    std::string ss = url.substr(1);
    node *matched_node = get_engine().node_get(ss);

    // get status of other nodes
    if (matched_node) {
        core::object::record data = {
            {"node", obj["node"].as_string()},
            {"type", "info"},
            {"info", enable_info},
            {"count", request_count++}
        };
        reply("httpctl", "http", data);
    }
    else {
        core::object::record data = {
            {"node", obj["node"].as_string()},
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
    if (c.is_reply(this)) {
        if (obj["type"].as_string() == "file") {
            reply("httpctl", "http", obj);
        }
    }
    else {
        if (obj["type"].as_string() == "http") {
            handle_get(obj);
        }
    }
}

void routectl::update() {}

}
