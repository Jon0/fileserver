#include <iostream>
#include <memory>

#include "http.h"

std::unique_ptr<core::node> hctl;

void http_init(core::engine &e) {
    std::cout << "http init\n";
    hctl = std::make_unique<http::httpctl>(e);
}

void http_uninit(core::engine &e) {
    std::cout << "http uninit\n";
    hctl = nullptr;
}

namespace http {

httpctl::httpctl(core::engine &e)
    :
    core::node(e, "httpctl") {
}

core::node *httpctl::match(const core::node &from, const std::string &type) {
    if (type == "binary") {
        return this;
    }
	return nullptr;
}

void httpctl::recieve(core::channel &c, const core::object &obj) {
    send_all(read_request(obj));

    core::object::record data = {
        {"type", "binary"},
        {"data", "HTTP/1.1 200 OK\nContent-Length: 5\n\nTest\n"}
    };
    c.reply(data);
}

void httpctl::update() {

}

core::object read_request(const core::object &obj) {
    std::vector<std::string> lines = split(obj["data"].as_string(), '\n');
    core::object::record data;
    data.insert(std::make_pair("type", "http"));

    // parse http request
    if (lines.empty()) {
        std::cout << "request is empty\n";
        return data;
    }

    auto first_line = split(lines[0], ' ');
    if (first_line.size() != 3) {
        std::cout << "first line requires 3 parts\n";
        return data;
    }

    data.insert(std::make_pair("verb", first_line[0]));
    data.insert(std::make_pair("url", first_line[1]));
    data.insert(std::make_pair("version", first_line[2]));
    for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
        auto attr_line = split(*i, ':');
        if (attr_line.size() == 2) {
            data.insert(std::make_pair(attr_line[0], attr_line[1]));
        }
    }

    return data;
}

}
