#pragma once

#include <thread>
#include <unordered_set>
#include <vector>

#include "channel.h"
#include "module.h"

namespace core {

class engine {
public:
    engine();
    ~engine();

    /**
     * start with a list of initial modules
     * might be useful to have a main loop class
     */
    void start();

    /**
     * make these nice for modules
     * so they don't get left open
     */
    void node_open(node *n);
    void node_close(node *n);

    /**
     * find matching nodes
     * objects may work better than strings
     */
    std::vector<node *> node_search(const node &from, const std::string &type);

    void thread_start(std::function<void()> handler);

private:
    void open_module(const std::string &module_path);

    void node_cleanup();

    std::vector<std::string> initial_modules;

    // list of open channels
    std::vector<node *> nodes;
    std::unordered_set<node *> to_remove;

    // list of connected modules
    std::vector<std::unique_ptr<module>> modules;

    // running threads
    std::vector<std::thread> thread_pool;

};

}
