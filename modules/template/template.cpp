#include <iostream>

#include "template.h"

std::unique_ptr<core::node> tctl;

void template_init(core::engine &e) {
    tctl = std::make_unique<tmpl::templatectl>(e);
}

void template_uninit(core::engine &e) {
    tctl = nullptr;
}

namespace tmpl {


templatectl::templatectl(core::engine &e)
    :
    core::node(e, "templatectl") {
}


core::object obj_template(const core::object &obj) {
    std::string page = "<table>\n";
    for (auto &o : obj.as_record()) {
        page += "<tr>" + o.first + "</tr>\n";
    }
    page += "</table>\n";

    core::object::record data = {
        {"node", obj["node"]},
        {"type", "http"},
        {"data", page},
    };
    return data;
}


core::object templatectl::page_template(const core::object &obj) {
    core::object::record data = {
        {"node", obj["node"]},
        {"type", "http"},
        {"data", obj["filedata"]},
    };
    return data;
}


void templatectl::create_notify(core::node *other) {

}


void templatectl::remove_notify(core::node *other) {

}


void templatectl::recieve(core::channel &c, const core::object &obj) {
    if (obj["type"].as_string() == "file") {
        event("http", page_template(obj));
    }
}


void templatectl::update() {

}


}
