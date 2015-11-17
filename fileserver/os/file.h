#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

namespace fserv {
namespace os {


void error(const char *msg);

class location {
public:
	location(const std::string &path);

	bool exists() const;

	bool isdir() const;

	bool isexec() const;

	int size() const;

	std::string sizestr() const;

	std::string modified() const;

	std::string mode() const;

	std::string path() const;

	location append(const std::string &path) const;

private:
	const std::string filepath;
	const int location_exists;
	struct stat location_stat;

};


class fdbuf : public std::streambuf {
public:
	typedef std::streambuf::traits_type traits_type;

	fdbuf(int fd);
	~fdbuf();
	void open(int fd);
	void close();

protected:
	int overflow(int c);
	int underflow();
	int sync();

private:
	enum { bufsize = 1024 };
	char outbuf_[bufsize];
	char inbuf_[bufsize + 16 - sizeof(int)];
	int  fd_;
};


/**
 * a file desciptor allows reading and writing
 * to various io devices
 * use fdbuf instead
 */
class fd_stream {
public:
	fd_stream(int fd);

	std::string readto(char end);

	std::string readlen(size_t length);

	void sendstr(const std::string &str) const;

	void sendfile(const std::string &filename) const;

private:
	const int fd;

	char readbuffer[1024];

	std::stringbuf sbuffer;
};


/**
 * A file directory
 */
class directory {
public:
	directory(const std::string &path);
	virtual ~directory();

	/**
	 * read all files with a given extension
	 */
	std::vector<std::string> file_list() const;

private:
	std::string path;

};


}
}
