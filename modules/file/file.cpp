#include <fstream>
#include <iostream>
#include <sstream>

#include <dirent.h>
#include <fcntl.h>
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


path::path(core::engine &e, const std::string &path, const std::string &ret)
	:
    core::node(e, path),
	filepath(path),
    return_node(ret),
    location_fd(open(path.c_str(), O_RDONLY)),
	location_exists(stat(path.c_str(), &location_stat)) {
}


bool path::exists() const {
    return location_exists >= 0;
}


bool path::isdir() const {
    return S_ISDIR(location_stat.st_mode);
}


bool path::ischr() const {
	return S_ISCHR(location_stat.st_mode);
}


bool path::isexec() const {
    return (location_stat.st_mode & S_IXUSR);
}


int path::size() const {
	return location_stat.st_size;
}


int path::device() const {
		return location_stat.st_rdev;
}


std::string path::sizestr() const {
	return std::to_string(size());
}

std::string path::modified() const {
	return std::to_string(location_stat.st_mtime);
}


std::string path::mode() const {
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


std::string path::str() const {
	return filepath;
}


std::string path::append(const std::string &pathstr) {
    if (filepath.back() != '/' && pathstr.front() != '/') {
    	return filepath + "/" + pathstr;
    }
    else {
        return filepath + pathstr;
    }
}


core::object path::as_object() const {

    // include directory content
    // or include filedata
    core::object::record data = {
            {"path", str()},
            {"exists", exists()},
            {"isdir", isdir()},
            {"mode", mode()},
            {"size", size()},
    };
    return data;
}


void path::create_notify(core::node *other) {}


void path::remove_notify(core::node *other) {}


void path::recieve(core::channel &c, const core::object &obj) {
    /**
     * metadata requests should be queued
     */
    core::object::record data = {
        {"type", "file"},
        {"node", return_node},
        {"metadata", as_object()},
    };
    event("file", data);
}


void path::update() {
    if (exists()) {
        // if (isdir()) {
        //     data.insert({"dirdata", read_dir(filepath)});
        // }
        // else {
        //     data.insert({"filedata", read_file(filepath)});
        // }
    }
    //event("file", data);
}


core::object path::transform(const core::object &obj) const {

}


filectl::filectl(core::engine &e)
    :
    core::node(e, "filectl") {
}


void filectl::create_notify(core::node *other) {}


void filectl::remove_notify(core::node *other) {}


void filectl::recieve(core::channel &c, const core::object &obj) {
    std::string pathstr = "." + obj["path"].as_string();
    if (!pathstr.empty()) {
        std::cout << "file request for " << pathstr << " recieved\n";
        paths.emplace_back(std::make_unique<path>(get_engine(), pathstr, obj["node"].as_string()));
        path *p = paths.back().get();
        p->channel_open("file", get_engine().node_get("templatectl"));
        p->msg(c, obj);
    }
}


void filectl::update() {}


core::object filectl::transform(const core::object &obj) const {

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
