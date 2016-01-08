#pragma once

#include <functional>
#include <string>

#include "object.h"

namespace core {

class channel;
class engine;

/**
 * connection between engine and a module, or 2 modules
 * there might be two classes, channel server and client
 * this should be implemented by core::engine and each module
 */
class node {
public:
    node(engine &e, const std::string &name);

    engine &get_engine();
    std::string get_name() const;

    // ensure updates can only access the owned channels
    std::vector<channel *> owned_channels() const;

    void msg(channel &remote, const object &obj);

    // these should be private implementations
    virtual void recieve(channel &remote, const object &obj) = 0;
    virtual void update() = 0;

private:
    engine &engi;

    /**
     * these should become objects
     */
    const std::string name;
    std::string in_type;
    std::string out_type;

};

/**
 * todo channel / edge class
 * this could also be extended by owning node
 */
class channel {
public:
    channel();

    const std::string name;
    std::string in_type;

    void send(const object &obj);
    void reply(const object &obj);

};

}
