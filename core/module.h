#pragma once

#include <string>

namespace core {

class engine;

std::string path_to_name(const std::string &module_path);

class module {
public:
    module(const std::string &module_path);
    ~module();


    bool is_open() const;
    std::string get_name() const;
    void init_module(engine &e) const;

private:
    const std::string lib_name;
	void *lib_handle;

};

}
