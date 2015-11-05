#pragma once

#include <string>
#include <unordered_map>

namespace fserv {
namespace http {


std::string create_response(const std::string &content);


const std::unordered_map<unsigned short, std::string> code_str = {
	{200, "OK"},
	{404, "Not Found"}
};


}
}
