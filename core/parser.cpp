#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "parser.h"

namespace core {

std::vector<std::string> lex(const std::string &fname) {
    std::vector<std::string> result;
    std::stringstream ss;
    std::ifstream fbuf(fname);
    ss << fbuf.rdbuf();
    std::regex r("\\{|\\}|->|:|[_[:alnum:]]+");
    std::string in = ss.str();
    std::smatch match;
    while (regex_search(in, match, r)) {
        for (int i = 0; i < match.size(); ++i) {
            result.push_back(match[i]);
        }
        in = match.suffix().str();
    }
	return result;
}

void read_file(const std::string &fname) {
    std::vector<std::string> source = lex(fname);

    for (auto &s : source) {
        std::cout << ": " << s << "\n";
    }
}

}
