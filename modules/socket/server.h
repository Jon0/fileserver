#pragma once

#include <memory>
#include <vector>
#include <core/engine.h>

#include "socket.h"

extern "C" {

void socket_init(core::engine &e);

void socket_uninit(core::engine &e);

}

namespace sock {

class socketctl : public core::node {
public:
    socketctl(core::engine &e);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;

private:
    std::vector<std::unique_ptr<tcp_acceptor>> acceptors;

};

}
