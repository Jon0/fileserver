#include <fstream>
#include <iostream>

#include <sstream>
#include <stdexcept>
#include <vector>

#include "strings.h"

namespace fserv {
namespace io {

int parse_int(const std::string &str, int default_value) {
	int result = default_value;
	try {
		result = stoi(str);
	}
	catch (const std::invalid_argument &) {}
	return result;
}

std::vector<std::string> split(const std::string &str, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (item.length() > 0) {
			result.push_back(item);
		}
	}
	return result;
}

std::string read_file(const std::string &filename) {
	std::ifstream t(filename);
	std::stringstream ss;
	ss << t.rdbuf();
	return ss.str();
}

std::vector<std::string> read_file_lines(const std::string &filename) {
	return split(read_file(filename), '\n');
}

std::unordered_map<std::string, std::string> read_file_map(const std::string &filename) {
	std::unordered_map<std::string, std::string> result;
	for (auto &line : read_file_lines(filename)) {
		std::vector<std::string> line_split = io::split(line, ' ');
		if (line_split.size() == 2) {
			result.insert(std::make_pair(line_split[0], line_split[1]));
		}
	}
	return result;
}

std::string read_line(const std::string &filename) {
	std::string content = read_file(filename);
	auto vector = split(content, '\n');
	if (vector.empty()) {
		return "";
	}
	return vector[0];
}

} // namespace io
}
