#pragma once

#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "channel.h"
#include "module.h"

namespace core {

class node_set {
public:
    node_set();

    /**
     * make these nice for modules
     * so they don't get left open
     */
    void node_open(node *n);
    void node_close(node *n);

    node *node_get(const std::string &name);

    void update();

private:
    void node_cleanup();

    // list of open nodes
    std::vector<node *> nodes;
    std::unordered_set<node *> to_create;
    std::unordered_set<node *> to_remove;
};


class engine {
public:
    engine();
    ~engine();

    /**
     * start with a list of initial modules
     * might be useful to have a main loop class
     */
    void start();
    void main_loop();

    // return the set of nodes
    node_set &nodes();

private:
    void open_module(const std::string &module_path);

    // all registered nodes
    node_set nds;

    // modules to automatically open
    std::vector<std::string> initial_modules;

    // list of connected modules
    std::vector<std::unique_ptr<module>> modules;

};

}
