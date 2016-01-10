#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <core/engine.h>

extern "C" {

void flow_init(core::engine &e);

void flow_uninit(core::engine &e);

}

namespace flow {

using flow_type = std::tuple<std::string, std::string, std::string>;

class flowctl : public core::node {
public:
    flowctl(core::engine &e);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;

private:
    void make_link(core::node *from, core::node *to, const std::string &type);

    // list nodes which need linking, and a type
    std::vector<flow_type> links;

};

}