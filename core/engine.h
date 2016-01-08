#pragma once

#include <vector>

#include "channel.h"
#include "module.h"

namespace core {

class engine {
public:
    engine();

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


    void start_process();
    void stop_process();

private:
    void open_module(const std::string &module_path);

    std::vector<std::string> initial_modules;

    // list of open channels
    std::vector<node *> nodes;

    // list of connected modules
    std::vector<module> modules;

};

}
