#pragma once

#include <sys/stat.h>
#include <unistd.h>
#include <core/engine.h>

extern "C" {

void file_init(core::engine &e);

void file_uninit(core::engine &e);

}

namespace file {


class path : public core::node {
public:
	path(core::node_set &e, const std::string &path, const std::string &ret);
	~path();

	bool exists() const;
	bool isdir() const;
	bool ischr() const ;
	bool isexec() const;
	bool read_complete() const;
	int size() const;
	int device() const;

	std::string sizestr() const;
	std::string modified() const;
	std::string mode() const;
	std::string str() const;
	std::string append(const std::string &pathstr);

    core::object as_object() const;

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;

private:
	const std::string filepath;
	const int location_fd;
    const int location_exists;
	struct stat location_stat;
	const std::string return_channel;
	const std::string return_node;
	bool completed;


};


class filectl : public core::node {
public:
    filectl(core::node_set &e);

    void create_notify(core::node *other) override;
    void remove_notify(core::node *other) override;
    void recieve(core::channel &c, const core::object &obj) override;
    void update() override;
    core::object transform(const core::object &obj) const override;

private:
    std::vector<std::unique_ptr<path>> paths;

};


bool readyfd(int fd);


core::object read_dir(const std::string &path);


}
