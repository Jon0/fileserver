#pragma once

#include <functional>
#include <string>

#include "object.h"

namespace core {

class channel;
class node_set;

/**
 * handler for reading and writing to external devices
 */
class endpoint {
public:
    virtual size_t read_block(char *buffer, size_t size) = 0;
};

/**
 * connection between engine and a module, or 2 modules
 * there might be two classes, channel server and client
 * this should be implemented by core::engine and each module
 */
class node {
public:
    node(node_set &e, const std::string &name);
    virtual ~node();

    node_set &get_set();
    std::string get_name() const;
    int channel_count() const;

    channel *channel_open(const std::string &type, node *other);
    void channel_close(node *other);
    void channel_copy_all(node *other);

    /**
     * register a channel this node will recieve from
     */
    void register_channel(channel *c);
    void deregister_channel(channel *c);


    // ensure updates can only access the owned channels
    std::vector<channel *> owned_channels() const;

    void send_all(const object &obj);
    void reply_all(const object &obj);

    // pass message to this object
    void msg(channel &remote, const object &obj);

    // process events
    void event(const std::string &type, const object &obj);
    void reply(const std::string &name, const std::string &type, const object &obj);

    // these should be private implementations
    virtual void create_notify(node *other) = 0;
    virtual void remove_notify(node *other) = 0;
    virtual void recieve(channel &c, const object &obj) = 0;
    virtual void update() = 0;

    // todo
    virtual object transform(const object &obj) const = 0;

private:
    node_set &ns;


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
    channel(const std::string &type, node &owner);

    node &owner;
    node *remote;

    /**
     * these could become objects
     */
    const std::string content_type;
    const std::string send_name;
    std::string reply_name;

    bool is_reply(node *other) const;

    std::string get_name() const;
    std::string get_type() const;

    node *get_reciever() const;
    void set_reciever(node *other);

    void send(const object &obj);
    void reply(const object &obj);

};

}
