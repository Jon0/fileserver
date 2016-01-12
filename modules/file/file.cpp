#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include <dirent.h>
#include <fcntl.h>
#include <poll.h>
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
    location_fd(open(path.c_str(), O_RDONLY)),
	location_exists(stat(path.c_str(), &location_stat)),
    return_channel(ischr() ? "chr" : "file"),
    return_node(ret),
    completed(!exists()) {
}

path::~path() {
    if (location_fd >= 0) {
        close(location_fd);
    }
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


bool path::read_complete() const {
    return completed;
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
            {"ischr", ischr()},
            {"mode", mode()},
            {"size", size()},
    };
    return data;
}


void path::create_notify(core::node *other) {}


void path::remove_notify(core::node *other) {}


void path::recieve(core::channel &c, const core::object &obj) {}


void path::update() {
    if (exists() && !completed) {
        core::object::record data = {
            {"type", "file"},
            {"node", return_node},
            {"metadata", as_object()},
        };
        if (isdir()) {
            data.insert({"dir", read_dir(filepath)});
            event(return_channel, data);
            completed = true;
        }
        else if (readyfd(location_fd)) {

            // report raw data
            constexpr int buf_size = 16 * 1024;
    		char buffer[buf_size];
    		int n = read(location_fd, buffer, buf_size);
    		if (n < 0) {
    			completed = true;
    			perror("ERROR on read");
    		}
    		else if (n == 0) {
    			completed = true;
    			std::cout << "reached eof\n";
    		}
    		else if (n) {
                std::cout << "read " << n << " bytes\n";
                data.insert({"data", std::string(buffer, n)});
    			event(return_channel, data);
    		}
        }
    }
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
        paths.back()->channel_copy_all(this);
    }
}


void filectl::update() {
    paths.erase(
        std::remove_if(
            paths.begin(),
            paths.end(),
            [](std::unique_ptr<path> &p) {
                return p->read_complete();
            }),
        paths.end()
    );
}


core::object filectl::transform(const core::object &obj) const {}


bool readyfd(int fd) {
    struct pollfd fds {fd, POLLIN, POLLIN};
    int result = poll(&fds, 1, 0);
	if (result == -1) {
		perror("ERROR on poll");
	}
    else if (result) {
    	return true;
	}
	return false;
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


}
