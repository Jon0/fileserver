#include <iostream>

#include <dlfcn.h>

#include "../io/strings.h"
#include "module.h"

namespace fserv {
namespace module {

module::module(const std::string &confpath, const std::string &libpath)
	:
	lib_handle(dlopen((confpath + libpath).c_str(), RTLD_LAZY)) {

	if (open()) {
		std::cout << "loaded " << libpath << "\n";

		// initialise configuration
		set_path(confpath.c_str());
	}
	else {
		std::cerr << "unable to load " << libpath << ": " << dlerror() << "\n";
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


void module::call(const std::string &fname) const {
	void (*fn)();
	char *error;

	fn = reinterpret_cast<void (*)()>(dlsym(lib_handle, fname.c_str()));
	if ((error = dlerror()) != NULL) {
		std::cerr << "error " << fname << ": " << error << "\n";
	}
	else {
		fn();
	}
}


void module::set_path(const char *path) const {
	void (*fn)(const char *);
	char *error;

	fn = reinterpret_cast<void (*)(const char *)>(dlsym(lib_handle, "set_conf_path"));
	if ((error = dlerror()) != NULL) {
		std::cerr << "error calling set_conf_path: " << error << "\n";
	}
	else {
		fn(path);
	}
}


const char *module::get_page(const char *page_name) const {
	const char *(*fn)(const char *);
	char *error;

	fn = reinterpret_cast<const char *(*)(const char *)>(dlsym(lib_handle, "get_page"));
	if ((error = dlerror()) != NULL) {
		std::cerr << "error calling get_page: " << error << "\n";
		return "not found";
	}
	else {
		return fn(page_name);
	}
}


module_config::module_config(const std::string &confpath)
	:
	conf_path(confpath),
	mod_paths(io::read_file_map(confpath + "/modules.conf")) {

}


bool module_config::has_module(const std::string &name) const {
	return (mod_paths.count(name) > 0);
}


std::string module_config::module_list() const {
	std::string result = "";
	for (auto &m : mod_paths) {
		result += m.first + " -> " + m.second + "\n";
	}
	return result;
}

const std::unordered_map<std::string, std::string> &module_config::module_map() const {
	return mod_paths;
}

void module_config::get_module(const std::string &name, std::function<void(module &)> callback) const {
	if (mod_paths.count(name) > 0) {
		module used_module(conf_path, mod_paths.at(name));
		if (used_module.open()) {
			callback(used_module);
		}
	}
}


}
}
