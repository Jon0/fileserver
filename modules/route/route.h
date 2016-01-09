#pragma once

#include <core/engine.h>

extern "C" {

void route_init(core::engine &e);

void route_uninit(core::engine &e);

}

namespace route {

class routectl : public core::node {
public:
    routectl(core::engine &e);

    core::node *match(const core::node &from, const std::string &type) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
};

}
