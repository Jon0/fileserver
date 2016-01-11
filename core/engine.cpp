#include <algorithm>
#include <chrono>
#include <iostream>

#include "engine.h"

namespace core {

engine::engine()
    :
    initial_modules({
        "modules/file/libfile.so",
        "modules/flow/libflow.so",
        "modules/http/libhttp.so",
        "modules/log/liblog.so",
        "modules/route/libroute.so",
        "modules/socket/libsocket.so",
        "modules/template/libtemplate.so",
    }) {}


engine::~engine() {}


void engine::start() {
    for (std::string &m : initial_modules) {
        open_module(m);
    }

    while (true) {
        node_cleanup();

        // read and respond etc...
        for (node *n : nodes) {
            if (to_remove.count(n) == 0) {
                //std::cout << "[engine] updating " << n->get_name() << "\n";
                n->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}


void engine::node_open(node *n) {
    std::cout << "[engine] opening node " << n->get_name() << "\n";
    to_create.insert(n);
}


void engine::node_close(node *n) {
    std::cout << "[engine] closing node " << n->get_name() << "\n";
    to_remove.insert(n);
    for (node *nd : nodes) {
        if (to_remove.count(nd) == 0) {
            nd->channel_close(n);
        }
    }
}


node *engine::node_get(const std::string &name) {
    for (node *n : nodes) {
        if (to_remove.count(n) == 0 && n->get_name() == name) {
            return n;
        }
    }
    return nullptr;
}


void engine::open_module(const std::string &module_path) {
    modules.emplace_back(std::make_unique<module>(module_path));
    modules.back()->init_module(*this);
}

void engine::node_cleanup() {

    // this will always occur before the new
    // node is updated or recieves data
    for (node *newnode : to_create) {
        for (node *existing : nodes) {
            if (to_remove.count(existing) == 0) {
                newnode->create_notify(existing);
                existing->create_notify(newnode);
            }
        }
        nodes.emplace_back(newnode);
    }

    for (node *rmnode : to_remove) {
        for (node *existing : nodes) {
            if (to_create.count(existing) == 0 &&
                to_remove.count(existing) == 0) {
                existing->remove_notify(rmnode);
            }
        }
    }
    nodes.erase(
        std::remove_if(
            nodes.begin(),
            nodes.end(),
            [this](node *n) {
                return to_remove.count(n) > 0;
            }),
        nodes.end()
    );
    to_create.clear();
    to_remove.clear();
}

}
