#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace fserv {
namespace http {

using str_map = std::unordered_map<std::string, std::string>;


/**
 * standard http request verbs
 */
enum class request_type {
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


class request {
public:
	request(request_type t, std::string l);

	std::string info() const;

	const request_type type;

	// requested url as string
	const std::string location;

	// ip and port of remote
	std::string remote;

	// additional variables
	str_map data;

};


/**
 * parsing an http request header and data
 */
request parse_request(const std::string &remote, const std::string &request_data);


}
}
