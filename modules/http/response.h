#pragma once

#include <string>
#include <unordered_map>

namespace http {


struct response_data {

	// http version
	std::string version;

	// response code
	int code;

	// mime type
	std::string type;
};


std::string basic_header(int code, const std::string &mime_type);


std::string create_response(const response_data &rd, const std::string &content);


std::string create_response(const std::string &content);


const std::unordered_map<unsigned short, std::string> code_str = {
	{200, "OK"},
	{404, "Not Found"},
	{501, "Not Implemented"}
};


}
