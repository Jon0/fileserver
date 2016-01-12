#pragma once

#include <core/engine.h>

extern "C" {

void template_init(core::engine &e);

void template_uninit(core::engine &e);

}

namespace tmpl {

class templatectl : public core::node {
public:
    templatectl(core::engine &e);

    core::object obj_template(const core::object &obj);
    core::object page_template(const core::object &obj);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;

private:
    int size;
    std::string content;

};

}
