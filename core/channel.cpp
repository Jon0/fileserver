#include <iostream>

#include "channel.h"

namespace core {


node::node(engine &e, const std::string &name)
    :
    engi(e),
    name(name) {

}


engine &node::get_engine() {
    return engi;
}


std::string node::get_name() const {
    return name;
}


std::vector<channel *> node::owned_channels() const {
    return std::vector<channel *>();
}

void node::msg(channel &remote, const object &obj) {
    std::cout << obj.str() << "\n";
    recieve(remote, obj);
}


channel::channel()
    :
    name("unnamed") {
}


void channel::send(const object &obj) {

}


void channel::reply(const object &obj) {

}


}
