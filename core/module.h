#pragma once

#include <string>

namespace core {

class engine;

class module {
public:
    module(const std::string &module_path);
    ~module();

    bool open() const;
    void init_module(engine &e) const;

private:
	void *lib_handle;

};

}
