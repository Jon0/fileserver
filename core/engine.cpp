#include <iostream>

#include "engine.h"

namespace core {

engine::engine()
    :
    initial_modules({
        "modules/log/liblog.so",
        "modules/socket/libsocket.so"
    }) {}

void engine::start() {
    for (std::string &m : initial_modules) {
        open_module(m);
    }

    // todo: initial messaging

    while (true) {
        std::cout << "main loop\n";
        // todo: read and respond etc...
    }
}

void engine::node_open(node *n) {
    //std::cout << "opening channel " << name << "\n";
    nodes.emplace_back(n);
    n->update();
}

void engine::node_close(node *n) {

}

void engine::open_module(const std::string &module_path) {
    modules.emplace_back(module_path);
    modules.back().init_module(*this);
}

}
