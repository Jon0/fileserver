#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "parser.h"

namespace core {


bool match(tokens &source, const std::string &t) {
    if (source.front() == t) {
        source.pop();
        return true;
    }
    return false;
}


alias read_alias(tokens &source) {
    std::vector<std::string> symbols;
    if (match(source, "enum")) {
        std::string name = source.front();
        source.pop();
        if (match(source, "{")) {
            while (!match(source, "}")) {
                symbols.push_back(source.front());
                source.pop();
            }
        }
        return alias{name, symbols};
    }
    return alias{"", symbols};
}


void read_mapping(tokens &source) {
    std::string name = source.front();
    source.pop();
    match(source, ":");
    if (match(source, "{")) {
        while (!match(source, "}")) {
            std::cout << "got " << source.front() << "\n";
            source.pop();
        }
    }
}


function read_func(tokens &source) {
    if (match(source, "func")) {
        std::string name = source.front();
        source.pop();
        if (match(source, "{")) {
            while (!match(source, "}")) {
                read_mapping(source);
            }
        }
        return function();
    }
    return function();
}


program read_file(const std::string &fname) {
    tokens source = lex(fname);
    program p;

    while (!source.empty()) {
        if (source.front() == "enum") {
            p.add_alias(read_alias(source));
        }
        else if (source.front() == "func") {
            p.add_function(read_func(source));
        }
        else {
            source.pop();
        }
    }
    return p;
}


tokens lex(const std::string &fname) {
    tokens result;
    std::stringstream ss;
    std::ifstream fbuf(fname);
    ss << fbuf.rdbuf();
    std::regex r("\\{|\\}|->|:|[_[:alnum:]]+");
    std::string in = ss.str();
    std::smatch match;
    while (std::regex_search(in, match, r)) {
        for (int i = 0; i < match.size(); ++i) {
            result.push(match[i]);
        }
        in = match.suffix().str();
    }
	return result;
}


}
