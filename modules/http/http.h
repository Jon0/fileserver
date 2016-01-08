#pragma once

#include <core/engine.h>

extern "C" {

void init(core::engine &e);

void uninit(core::engine &e);

}

namespace http {

class httpctl : public core::node {
public:
    httpctl(core::engine &e);

    void recieve(core::channel &remote, const core::object &obj) override;
    void update() override;
};


}
