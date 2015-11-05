#include <string>

#include "response.h"

namespace fserv {
namespace http {

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
