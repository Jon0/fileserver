#include <iostream>
#include <dlfcn.h>

#include "engine.h"
#include "module.h"

namespace core {

module::module(const std::string &module_path)
    :
    lib_handle(dlopen(module_path.c_str(), RTLD_LAZY)) {

	if (open()) {
		std::cout << "loaded " << module_path << "\n";

		// initialise configuration
		//set_path(confpath.c_str());
	}
	else {
		std::cerr << "unable to load " << module_path << ": " << dlerror() << "\n";
	}
}


module::~module() {
	if (open()) {
		dlclose(lib_handle);
	}
}


bool module::open() const {
	return lib_handle;
}


void module::init_module(engine &e) const {
	void (*init)(core::engine &);
	char *error;

	init = reinterpret_cast<void (*)(core::engine &)>(dlsym(lib_handle, "init"));
	if ((error = dlerror()) != NULL) {
		std::cerr << "init error: " << error << "\n";
	}
	else {
		init(e);
	}
}


}
