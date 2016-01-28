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


state_space::ptr_t read_type(type_context &ct, tokens &source) {
    std::string next = source.front();
    if (ct.contains(next)) {
        state_space::ptr_t lhs = ct.get_type(next);
        source.pop();
        if (match(source, "*")) {
            state_space::ptr_t rhs = read_type(ct, source);
            std::vector<state_space::ptr_t> elements({lhs, rhs});
            return std::make_shared<state_multiply>(elements);
        }
        else if (match(source, "->")) {
            state_space::ptr_t rhs = read_type(ct, source);
            return std::make_shared<state_function>(lhs, rhs);
        }
        else {
            return lhs;
        }
    }
    else if (match(source, "{")) {
        std::vector<std::string> symbols;
        while (!match(source, "}")) {
            symbols.push_back(source.front());
            source.pop();
        }
        return std::make_shared<state_enum>(symbols);
    }
    std::cout << "read type failed\n";
    return nullptr;
}


void read_alias(type_context &ct, tokens &source) {
    std::vector<std::string> symbols;
    if (match(source, "alias")) {
        std::string name = source.front();
        source.pop();
        ct.insert(name, read_type(ct, source));
    }
}


void read_mapping(type_context &ct, tokens &source) {
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


void read_value(type_context &ct, tokens &source) {
    state_space::ptr_t type = read_type(ct, source);
    match(source, ":");
    if (match(source, "(")) {
        while (!match(source, ")")) {
            std::cout << "list value " << source.front() << "\n";
            source.pop();
        }
    }
    else {
        std::cout << "value " << source.front() << "\n";
        source.pop();
    }
}


void read_func(program &p, type_context &ct, tokens &source) {
    if (match(source, "elem")) {
        std::string name = source.front();
        source.pop();
        std::cout << "element " << name << "\n";
        read_value(ct, source);
        //p.add_function(name, function(type));
    }
}


program read_file(const std::string &fname) {
    tokens source = lex(fname);
    program p;
    type_context ct;

    while (!source.empty()) {
        if (source.front() == "alias") {
            read_alias(ct, source);
        }
        else if (source.front() == "elem") {
            read_func(p, ct, source);
        }
        else {
            std::cout << "cannot match " << source.front() << "\n";
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
    std::regex r("\\{|\\}|\\(|\\)|\\*|,|->|:|[_[:alnum:]]+");
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
