#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace http {


using str_map = std::unordered_map<std::string, std::string>;


/**
 * standard http request verbs
 */
enum class request_type {
	http_unknown,
	http_get,
	http_post,
	http_put,
	http_delete
};


const std::unordered_map<std::string, request_type> request_str = {
	{"GET", request_type::http_get},
	{"POST", request_type::http_post},
	{"PUT", request_type::http_put},
	{"DELETE", request_type::http_delete},
};

struct request_data {
	request_type type;

	// requested url as string
	std::string location;

	// http version
	std::string version;

	// additional variables
	str_map attributes;
};

class request {
public:
	request();
	request(const request_data &d);

	std::string info() const;

	const bool complete;

	const request_data data;
};

std::vector<std::string> read_lines(std::istream &in);

request read_request(std::istream &in);

}
