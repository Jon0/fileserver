#include <iostream>

#include "engine.h"

namespace core {

engine::engine()
    :
    initial_modules({
        "modules/log/liblog.so",
        "modules/server/libserver.so"
    }) {}

void engine::start() {
    for (std::string &m : initial_modules) {
        open_module(m);
    }
    for (channel *c : channels) {
        c->msg_module(object("accept"));
    }

    while (true) {
        // todo: read and respond etc...
    }
}

void engine::channel_open(channel *c) {
    //std::cout << "opening channel " << name << "\n";
    channels.emplace_back(c);
    c->update();
}

void engine::channel_close(channel *c) {

}

void engine::open_module(const std::string &module_path) {
    modules.emplace_back(module_path);
    modules.back().init_module(*this);
}

}
