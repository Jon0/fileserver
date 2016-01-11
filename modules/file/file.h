#pragma once

#include <sys/stat.h>
#include <unistd.h>
#include <core/engine.h>

extern "C" {

void file_init(core::engine &e);

void file_uninit(core::engine &e);

}

namespace file {


class filectl : public core::node {
public:
    filectl(core::engine &e);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
};


class location {
public:
	location(const std::string &path);

	bool exists() const;

	bool isdir() const;

	bool ischr() const ;

	bool isexec() const;

	int size() const;

	int device() const;

	std::string sizestr() const;

	std::string modified() const;

	std::string mode() const;

	std::string path() const;

	location append(const std::string &path) const;

    core::object as_object() const;

private:
	const std::string filepath;
	const int location_exists;
	struct stat location_stat;

};

core::object read_dir(const std::string &path);


/**
 * read a whole file into a string
 */
std::string read_file(const std::string &filename);


}
