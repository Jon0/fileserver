#pragma once

#include <core/engine.h>

extern "C" {

void http_init(core::engine &e);

void http_uninit(core::engine &e);

}

namespace http {

class httpctl : public core::node {
public:
    httpctl(core::node_set &e);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &remote, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;
};


/**
 * string split function using a char delimiter
 */
std::vector<std::string> split(const std::string &str, char delim);

core::object read_response(const core::object &obj);

core::object read_request(const core::object &obj);


}
