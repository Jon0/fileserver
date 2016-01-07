#include <iostream>

#include "channel.h"

namespace core {


channel::channel(engine &e, const std::string &name)
    :
    engi(e),
    name(name) {

}


engine &channel::get_engine() {
    return engi;
}


void channel::msg_engine(const object &obj) {
    // todo
    std::cout << obj.str() << "\n";
}


}
