#include <iostream>

#include <dirent.h>
#include <sys/types.h>

#include "file.h"

namespace fserv {
namespace os {


void error(const char *msg) {
	perror(msg);
	exit(1);
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


fdbuf::fdbuf(int fd)
	:
	fd_(-1) {
	this->open(fd);
}


fdbuf::~fdbuf() {
	this->close();
}


void fdbuf::open(int fd) {
	this->close();
	this->fd_ = fd;
	this->setg(this->inbuf_, this->inbuf_, this->inbuf_);
	this->setp(this->outbuf_, this->outbuf_ + bufsize - 1);
}


void fdbuf::close() {
	if (!(this->fd_ < 0)) {
		this->sync();
		::close(this->fd_);
	}
}


int fdbuf::overflow(int c) {
	if (!traits_type::eq_int_type(c, traits_type::eof())) {
		*this->pptr() = traits_type::to_char_type(c);
		this->pbump(1);
	}
	return this->sync() == -1
		? traits_type::eof()
		: traits_type::not_eof(c);
}


int fdbuf::sync() {
	if (this->pbase() != this->pptr()) {
		std::streamsize size(this->pptr() - this->pbase());
		std::streamsize done(::write(this->fd_, this->outbuf_, size));

		// The code below assumes that it is success if the stream made
		// some progress. Depending on the needs it may be more
		// reasonable to consider it a success only if it managed to
		// write the entire buffer and, e.g., loop a couple of times
		// to try achieving this success.
		if (done > 0) {
			std::copy(this->pbase() + done, this->pptr(), this->pbase());
			this->setp(this->pbase(), this->epptr());
			this->pbump(size - done);
		}
		else {
			std::cout << "fd " << fd_ << ": write error " << done << "\n";
			return -1;
		}
	}
	return this->pptr() != this->epptr()? 0: -1;
}


int fdbuf::underflow() {
	if (this->gptr() == this->egptr()) {
		std::streamsize pback(std::min(this->gptr() - this->eback(),
									   std::ptrdiff_t(16 - sizeof(int))));
		std::copy(this->egptr() - pback, this->egptr(), this->eback());
		int done(::read(this->fd_, this->eback() + pback, bufsize));
		if (done < 0) {
			std::cout << "fd " << fd_ << ": read error " << done << "\n";
		}
		this->setg(this->eback(),
				   this->eback() + pback,
				   this->eback() + pback + std::max(0, done));
	}
	return this->gptr() == this->egptr()
		? traits_type::eof()
		: traits_type::to_int_type(*this->gptr());
}


directory::directory(const std::string &path)
	:
	path(path) {
}


directory::~directory() {}


std::vector<std::string> directory::file_list() const {
	std::vector<std::string> names;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string filename = ent->d_name;
			if (!filename.empty()) {
				names.push_back(filename);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
	}

	return names;
}


}
}
