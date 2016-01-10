#include <iostream>
#include <string>
#include <sstream>

#include "../io/strings.h"
#include "request.h"

namespace fserv {
namespace http {


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

	auto first_line = io::split(lines[0], ' ');
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
		auto attr_line = io::split(*i, ':');
		if (attr_line.size() == 2) {
			data.attributes.insert(std::make_pair(attr_line[0], attr_line[1]));
		}
	}

	return request(data);
}


request parse_request(const std::string &remote, const std::string &req_lines) {
	auto lines = io::split(req_lines, '\n');
	request_data rd;

	// parse lines in the request
	bool read_data = false;
	for (auto &line : lines) {
		if (read_data) {
			// read request data past the header
			auto items = io::split(line, '&');
			for (auto &item : items) {
				auto key_value = io::split(item, '=');
				if (key_value.size() >= 2) {
					rd.attributes[key_value[0]] = key_value[1];
				}
			}
		}
		else {
			// read http header
			auto items = io::split(line, ' ');

			// if line starts with an http verb
			if (items.size() == 3 && request_str.count(items[0]) > 0) {

				// parse verb type
				rd.type = request_str.at(items[0]);

				// look for extra components
				auto components = io::split(items[1], '?');
				rd.location = components[0];

				// parse url arguments
				if (components.size() > 1) {
					auto key_value = io::split(components[1], '=');
					if (key_value.size() >= 2) {
						rd.attributes[key_value[0]] = key_value[1];
					}
				}
				rd.version = components[2];
			}
			else if (items.size() > 1 && items[0] == "Cookie:") {
				auto cookies = io::split(items[1], '\r');
				for (auto &c : cookies) {
					auto key_value = io::split(c, '=');
					if (key_value.size() >= 2) {
						rd.attributes[key_value[0]] = key_value[1];
					}
				}
			}
			else if (items.size() == 1) {
				// end of http header
				read_data = true;
			}
		}
	}

	// create and return request
	return request(rd);
}


}
}
