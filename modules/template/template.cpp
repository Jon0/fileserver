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


core::object templatectl::obj_template(const core::object &obj) {
    std::string result;
    result += "<h1>Index of " + obj["metadata"]["path"].as_string() + "</h1>";
    result += "<h2>Type " + obj["type"].as_string() + "</h2>";
    result += "<table>";
    result += "<tr>";
    result += "<th valign=\"top\"></th><th><a>Name</a></th><th><a>Last modified</a></th><th><a>Size</a></th><th><a>Description</a></th>";
    result += "</tr>";
    result += "<tr><th colspan=\"5\"><hr></th></tr>";

    for (auto &s : obj.as_record()) {
        std::string link = obj["metadata"]["path"].as_string();
        result += "<tr>";
        result += "<td valign=\"top\"></td><td><a href="+ link +">" + s.first + "</a></td>";
        result += "<td align=\"right\">?</td>";
        result += "<td align=\"right\">?</td>";
            result += "<td align=\"right\">?</td>";
            result += "</tr>";
    }
    result += "<tr><th colspan=\"5\"><hr></th></tr>";
    result += "</table>";

    core::object::record data = {
        {"node", obj["node"]},
        {"type", "http"},
        {"data", result},
        {"metadata", obj["metadata"]},
    };
    return data;
}


core::object templatectl::page_template(const core::object &obj) {
    if (obj["data"].is_null()) {
        return obj_template(obj);
    }
    else {
        core::object::record data = {
            {"node", obj["node"]},
            {"type", "http"},
            {"data", obj["data"]},
            {"metadata", obj["metadata"]},
        };

        // Todo, replace variables

        return data;
    }
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


core::object templatectl::transform(const core::object &obj) const {

}


}
