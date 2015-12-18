#include <string>

#include "response.h"

namespace fserv {
namespace http {


std::string create_response(const response_data &rd, const std::string &content) {
	std::string header = "";
	header += "HTTP/1.1 " + std::to_string(rd.code) + " ";
	header += code_str.at(rd.code) + "\n";

	if (!rd.type.empty()) {
		header += "Content-Type: " + rd.type + "\n";
	}

	int content_length = content.length();
	header += "Content-Length: " + std::to_string(content_length) + "\n";
	header += "\n";
	return header + content;
}


std::string create_response(const std::string &content) {
	std::string header = "";
	int response_code = 200;
	header += "HTTP/1.1 " + std::to_string(response_code) + " ";
	header += code_str.at(response_code) + "\n";

	std::string type = "text/html";
	if (!type.empty()) {
		header += "Content-Type: " + type + "\n";
	}

	int content_length = content.length();
	header += "Content-Length: " + std::to_string(content_length) + "\n";
	header += "\n";
	return header + content;
}


}
}
