#include <iostream>
#include <string>
#include <sstream>

#include "request.h"

namespace http {

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

request::request()
	:
	complete(false),
	data() {
}


request::request(const request_data &d)
	:
	complete(true),
	data(d) {
}


std::string request::info() const {
	std::string result;
	result += "type: " + std::to_string(static_cast<int>(data.type)) + "\n";
	result += "location: " + data.location + "\n";
	result += "version: " + data.version + "\n";
	for (auto &pair : data.attributes) {
		result += pair.first + ": " + pair.second + "\n";
	}
	return result;
}


std::vector<std::string> read_lines(std::istream &in) {
	std::vector<std::string> lines;

	// read until an empty line is reached
	std::string str_part = "";
	while (std::getline(in, str_part, '\n')) {

		// remove return character from lines
		int last_char = str_part.length() - 1;
		if (last_char >= 0 && str_part[last_char] == '\r') {
			str_part = str_part.substr(0, last_char);
		}

		// add non empty lines to the list
		if (str_part.length() == 0) {
			break;
		}
		else {
			lines.push_back(str_part);
		}
	}
	return lines;
}


request read_request(std::istream &in) {
	std::vector<std::string> lines = read_lines(in);

	// parse http request
	if (lines.empty()) {
		std::cout << "request is empty\n";
		return request();
	}

	auto first_line = split(lines[0], ' ');
	if (first_line.size() != 3) {
		std::cout << "first line requires 3 parts\n";
		return request();
	}

	// parse verb type
	request_data data;
	if (request_str.count(first_line[0]) > 0) {
		data.type = request_str.at(first_line[0]);
	}
	else {
		data.type = request_type::http_unknown;
	}
	data.location = first_line[1];
	data.version = first_line[2];

	for (auto i = lines.begin() + 1; i != lines.end(); ++i) {
		auto attr_line = split(*i, ':');
		if (attr_line.size() == 2) {
			data.attributes.insert(std::make_pair(attr_line[0], attr_line[1]));
		}
	}

	return request(data);
}


}
