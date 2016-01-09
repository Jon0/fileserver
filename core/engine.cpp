#include <algorithm>
#include <chrono>
#include <iostream>

#include "engine.h"

namespace core {

engine::engine()
    :
    initial_modules({
        "modules/file/libfile.so",
        "modules/http/libhttp.so",
        "modules/log/liblog.so",
        "modules/route/libroute.so",
        "modules/socket/libsocket.so",
    }) {}

engine::~engine() {
    for (std::thread &t : thread_pool) {
        t.join();
    }
}

void engine::start() {
    for (std::string &m : initial_modules) {
        open_module(m);
    }

    while (true) {
        // read and respond etc...
        for (node *n : nodes) {
            std::cout << "[engine] updating " << n->get_name() << "\n";
            n->update();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void engine::node_open(node *n) {
    std::cout << "[engine] opening node " << n->get_name() << "\n";
    nodes.emplace_back(n);
}

void engine::node_close(node *n) {
    std::cout << "[engine] closing node " << n->get_name() << "\n";
    for (node *nd : nodes) {
        nd->remove_notify(n);
    }
    nodes.erase(
        std::remove_if(
            nodes.begin(),
            nodes.end(),
            [n](node *nd) {
                return n == nd;
            }),
        nodes.end()
    );
}

std::vector<node *> engine::node_search(const node &from, const std::string &type) {
    std::cout << "[engine] searching for " << type << "\n";
    std::vector<node *> result;
    for (node *n : nodes) {
        node *derived = n->match(from, type);
        if (derived) {
            std::cout << "[engine] found " << derived->get_name() << "\n";
            result.push_back(derived);
        }
    }
    return result;
}

void engine::thread_start(std::function<void()> handler) {
    thread_pool.push_back(std::thread(handler));
}

void engine::open_module(const std::string &module_path) {
    modules.emplace_back(std::make_unique<module>(module_path));
    modules.back()->init_module(*this);
}

}
