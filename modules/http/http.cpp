#include "http.h"

void init(core::engine &e) {

}

void uninit(core::engine &e) {

}

namespace http {

httpctl::httpctl(core::engine &e)
    :
    core::node(e, "httpctl") {
}

void httpctl::recieve(core::channel &remote, const core::object &obj) {

}

void httpctl::update() {

}

}
