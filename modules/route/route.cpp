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


core::node *routectl::match(const core::node &from, const std::string &type) {
    if (type == "http") {
        return this;
    }
    return nullptr;
}


void routectl::recieve(core::channel &c, const core::object &obj) {
    if (obj.type() == core::value_type::record_type) {
        if (obj["type"].as_string() == "http") {
            std::string url = obj["url"].as_string();
            std::cout << "request for " << url << " recieved\n";

            // find a recieving channel
            for (auto n : get_engine().node_search(*this, "path")) {
                channel_open(n);
            }
            core::object::record data = {
                {"type", "path"},
                {"source", obj},
                {"path", url},
                {"action", "metadata"},
            };
            send_all(data);
        }
        else {
            reply_all(obj);
        }
    }
}

void routectl::update() {

}

}
