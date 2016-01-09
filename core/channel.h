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
    ~node();

    engine &get_engine();
    std::string get_name() const;

    channel *channel_open(node *other);
    void register_channel(channel *c);
    void remove_notify(node *other);

    // ensure updates can only access the owned channels
    std::vector<channel *> owned_channels() const;

    void send_all(const object &obj);
    void reply_all(const object &obj);

    void msg(channel &remote, const object &obj);

    // these should be private implementations
    virtual node *match(const node &from, const std::string &type) = 0;
    virtual void recieve(channel &c, const object &obj) = 0;
    virtual void update() = 0;

private:
    engine &engi;


    const std::string name;

    /**
     * channels owned by this
     */
    std::vector<std::unique_ptr<channel>> channels;

    /**
     * channels not owned by this
     */
    std::vector<channel *> reply_channels;

};

/**
 * todo channel / edge class
 * this could also be extended by owning node
 */
class channel {
public:
    channel(node &owner);

    node &owner;
    node *remote;
    const std::string name;

    /**
     * these could become objects
     */
    std::string content_type;
    std::string in_type;
    std::string out_type;

    node *get_reciever() const;
    void set_reciever(node *other);

    void send(const object &obj);
    void reply(const object &obj);

};

}
