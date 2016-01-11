#include <fstream>
#include <iostream>
#include <sstream>

#include <dirent.h>
#include <sys/types.h>

#include "file.h"

std::unique_ptr<core::node> fctl;

void file_init(core::engine &e) {
    fctl = std::make_unique<file::filectl>(e);
}

void file_uninit(core::engine &e) {
    fctl = nullptr;
}

namespace file {


filectl::filectl(core::engine &e)
    :
    core::node(e, "filectl") {
}


void filectl::create_notify(core::node *other) {

}


void filectl::remove_notify(core::node *other) {

}


void filectl::recieve(core::channel &c, const core::object &obj) {
    std::string path = "." + obj["path"].as_string();
    if (!path.empty()) {
        std::cout << "file request for " << path << " recieved\n";
        location loc(path);
        core::object::record data = {
            {"type", "file"},
            {"node", obj["node"].as_string()},
            {"metadata", loc.as_object()},
        };
        if (loc.exists()) {
            if (loc.isdir()) {
                data.insert({"dirdata", read_dir(path)});
            }
            else {
                data.insert({"filedata", read_file(path)});
            }
        }
        event("file", data);
    }
}


void filectl::update() {

}


location::location(const std::string &path)
	:
	filepath(path),
	location_exists(stat(path.c_str(), &location_stat)) {
}


bool location::exists() const {
	return location_exists >= 0;
}


bool location::isdir() const {
	return S_ISDIR(location_stat.st_mode);
}


bool location::ischr() const {
	return S_ISCHR(location_stat.st_mode);
}


bool location::isexec() const {
	return (location_stat.st_mode & S_IXUSR);
}


int location::size() const {
	return location_stat.st_size;
}


int location::device() const {
		return location_stat.st_rdev;
}


std::string location::sizestr() const {
	return std::to_string(size());
}

std::string location::modified() const {
	return std::to_string(location_stat.st_mtime);
}


std::string location::mode() const {
	std::string result;
	result += ( isdir() ? "d" : "-");
    result += ( (location_stat.st_mode & S_IRUSR) ? "r" : "-");
    result += ( (location_stat.st_mode & S_IWUSR) ? "w" : "-");
    result += ( (location_stat.st_mode & S_IXUSR) ? "x" : "-");
    result += ( (location_stat.st_mode & S_IRGRP) ? "r" : "-");
    result += ( (location_stat.st_mode & S_IWGRP) ? "w" : "-");
    result += ( (location_stat.st_mode & S_IXGRP) ? "x" : "-");
    result += ( (location_stat.st_mode & S_IROTH) ? "r" : "-");
    result += ( (location_stat.st_mode & S_IWOTH) ? "w" : "-");
    result += ( (location_stat.st_mode & S_IXOTH) ? "x" : "-");
    return result;
}


std::string location::path() const {
	return filepath;
}


location location::append(const std::string &path) const {
	if (filepath.back() != '/' && path.front() != '/') {
		return location(filepath + "/" + path);
	}
	else {
		return location(filepath + path);
	}
}


core::object location::as_object() const {

    // include directory content
    // or include filedata
    core::object::record data = {
            {"path", path()},
            {"exists", exists()},
            {"isdir", isdir()},
            {"mode", mode()},
    };
    return data;
}


core::object read_dir(const std::string &path) {
    core::object::array filenames;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string filename = ent->d_name;
			if (!filename.empty()) {
				filenames.push_back(filename);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
	}

	return filenames;
}


std::string read_file(const std::string &filename) {
	std::ifstream t(filename);
	std::stringstream ss;
	ss << t.rdbuf();
	return ss.str();
}


}
