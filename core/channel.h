#pragma once

#include <functional>
#include <string>

#include "object.h"

namespace core {

class engine;

/**
 * connection between engine and a module, or 2 modules
 * there might be two classes, channel server and client
 */
class channel {
public:
    channel(engine &e, const std::string &name);
    engine &get_engine();

    void msg_engine(const object &obj);
    virtual void msg_module(const object &obj) = 0;
    virtual void update() = 0;

private:
    engine &engi;

    /**
     * these should become objects
     */
    std::string name;
    std::string in_type;
    std::string out_type;

    std::function<void(channel &)> callback;

};

}
