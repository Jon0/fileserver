#include <iostream>
#include <memory>

#include "http.h"

std::unique_ptr<core::node> hctl;

void http_init(core::engine &e) {
    hctl = std::make_unique<http::httpctl>(e);
}

void http_uninit(core::engine &e) {
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
    if (obj["type"].as_string() == "binary") {
        //for (auto n : get_engine().node_search(*this, "http")) {
        //    channel_open(n);
        //}
        //send_all(read_request(obj));
        c.reply(read_response(read_request(obj)));
    }
    else {
        //reply_all(read_response(obj));
    }
}

void httpctl::update() {

}

core::object read_response(const core::object &obj) {
    core::object::record data;
    data.insert(std::make_pair("type", "binary"));
    std::string content = obj.str();
    std::string header = "HTTP/1.1 200 OK\n";
    header += "Content-Length: " + std::to_string(content.length()) + "\n";
    header += "\n";
    data.insert(std::make_pair("data", header + content));
    return data;
}


core::object read_request(const core::object &obj) {
    std::vector<std::string> lines = split(obj["data"].as_string(), '\n');
    core::object::record data;
    data.insert(std::make_pair("type", "http"));
    data.insert(std::make_pair("source", obj));

    // parse http request
    if (lines.empty()) {
        std::cout << "request is empty\n";
        return data;
    }

    std::string first_str = lines[0];
    if (first_str.back() == '\r') {
        first_str.pop_back();
    }
    auto first_line = split(first_str, ' ');

    if (first_line.size() != 3) {
        std::cout << "first line requires 3 parts\n";
        return data;
    }

    data.insert(std::make_pair("verb", first_line[0]));
    data.insert(std::make_pair("url", first_line[1]));
    data.insert(std::make_pair("version", first_line[2]));
    for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
        std::string line_str = *i;
        if (line_str.back() == '\r') {
            line_str.pop_back();
        }
        auto attr_line = split(line_str, ':');
        if (attr_line.size() == 2) {
            std::string value = attr_line[1];
            if (value[0] == ' ') {
                value = value.substr(1);
            }
            data.insert(std::make_pair(attr_line[0], value));
        }
    }
    return data;
}

}
