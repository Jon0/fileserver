#pragma once

#include <core/engine.h>

extern "C" {

void http_init(core::engine &e);

void http_uninit(core::engine &e);

}

namespace http {

class httpctl : public core::node {
public:
    httpctl(core::engine &e);

    core::node *match(const core::node &from, const std::string &type) override;
    void recieve(core::channel &remote, const core::object &obj) override;
    void update() override;
};


/**
 * string split function using a char delimiter
 */
std::vector<std::string> split(const std::string &str, char delim);


core::object read_request(const core::object &obj);


}
