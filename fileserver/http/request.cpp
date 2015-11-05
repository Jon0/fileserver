#include <string>
#include <sstream>

#include "../io/strings.h"
#include "request.h"

namespace fserv {
namespace http {

request::request(request_type t, std::string l)
	:
	type(t),
	location(l) {
}

std::string request::info() const {
	std::string result;
	result += "remote : " + remote + "\n";
	result += "type : " + std::to_string(static_cast<int>(type)) + "\n";
	result += "location : " + location + "\n";
	for (auto &pair : data) {
		result += pair.first + " : " + pair.second + "\n";
	}
	return result;
}


request parse_request(const std::string &remote, const std::string &request_data) {
	auto lines = io::split(request_data, '\n');

	request_type type;
	std::string location;
	str_map data;

	// parse lines in the request
	bool read_data = false;
	for (auto &line : lines) {
		if (read_data) {
			// read request data past the header
			auto items = io::split(line, '&');
			for (auto &item : items) {
				auto key_value = io::split(item, '=');
				if (key_value.size() >= 2) {
					data[key_value[0]] = key_value[1];
				}
			}
		}
		else {
			// read http header
			auto items = io::split(line, ' ');

			// if line starts with an http verb
			if (items.size() > 1 && request_str.count(items[0]) > 0) {

				// parse verb type
				type = request_str.at(items[0]);

				// look for extra components
				auto components = io::split(items[1], '?');
				location = components[0];

				// parse url arguments
				if (components.size() > 1) {
					auto key_value = io::split(components[1], '=');
					if (key_value.size() >= 2) {
						data[key_value[0]] = key_value[1];
					}
				}
			}
			else if (items.size() > 1 && items[0] == "Cookie:") {
				auto cookies = io::split(items[1], '\r');
				for (auto &c : cookies) {
					auto key_value = io::split(c, '=');
					if (key_value.size() >= 2) {
						data[key_value[0]] = key_value[1];
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
	request req(type, location);
	req.data = data;
	req.remote = remote;
	return req;
}


}
}
