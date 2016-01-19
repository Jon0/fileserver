#pragma once

#include <core/engine.h>

extern "C" {

void route_init(core::engine &e);

void route_uninit(core::engine &e);

}

namespace route {

class routectl : public core::node {
public:
    routectl(core::node_set &e);

    void handle_get(const core::object &obj);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;

private:
    bool enable_info;
    int request_count;

};

}
