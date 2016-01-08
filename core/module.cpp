#include <iostream>
#include <dlfcn.h>

#include "engine.h"
#include "module.h"

namespace core {


std::string path_to_name(const std::string &module_path) {
    auto start = module_path.find("lib");
    auto end = module_path.find(".so");
    if (start != std::string::npos && end != std::string::npos) {
        start += 3;
        return module_path.substr(start, end - start);
    }
    return "";
}


module::module(const std::string &module_path)
    :
    lib_name(path_to_name(module_path)),
    lib_handle(dlopen(module_path.c_str(), RTLD_LAZY)) {

	if (is_open()) {
		std::cout << "[" << lib_name << "] loaded\n";
	}
	else {
		std::cerr << "unable to load " << module_path << ": " << dlerror() << "\n";
	}
}


module::~module() {
	if (is_open()) {
        std::cout << "[" << lib_name << "] unloaded\n";
		dlclose(lib_handle);
	}
}


bool module::is_open() const {
	return lib_handle;
}


std::string module::get_name() const {
    return lib_name;
}


void module::init_module(engine &e) const {
	void (*init)(core::engine &);
	char *error;

    std::string init_name = lib_name + "_init";
	init = reinterpret_cast<void (*)(core::engine &)>(dlsym(lib_handle, init_name.c_str()));
	if ((error = dlerror()) != NULL) {
		std::cerr << "init error: " << error << "\n";
	}
	else {
		init(e);
	}
}


}
