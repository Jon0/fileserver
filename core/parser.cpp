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
    state_space::ptr_t current = nullptr;

    if (match(source, "(")) {
        current = read_type(ct, source);
        match(source, ")");
    }
    else if (match(source, "{")) {
        std::vector<std::string> symbols;
        while (!match(source, "}")) {
            symbols.push_back(source.front());
            source.pop();
        }
        current = std::make_shared<state_enum>(symbols);
    }
    else if (ct.contains(next)) {
        current = ct.get_type(next);
        source.pop();
    }
    else {
        current = state_space::empty_set;
    }

    // parse operators
    while (match(source, "*")) {
        state_space::ptr_t rhs = read_type(ct, source);
        std::vector<state_space::ptr_t> elements({current, rhs});
        current = std::make_shared<state_multiply>(elements);
    }

    while (match(source, "->")) {
        state_space::ptr_t rhs = read_type(ct, source);
        current = std::make_shared<state_function>(current, rhs);
    }

    return current;
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


symbol::ptr_t read_value(state_space::ptr_t type, type_context &ct, tokens &source) {
    auto type_size = static_cast<long unsigned int>(type->bytes());
    std::unique_ptr<char[]> b = std::make_unique<char[]>(type_size);

    if (match(source, "{")) {

        // read function mappings
        while (!match(source, "}")) {
            auto r_size = static_cast<long unsigned int>(type->rhs()->bytes());
            symbol::ptr_t lhs = read_value(type->lhs(), ct, source);
            if (match(source, "->")) {
                state_size_t index = lhs->index();
                symbol::ptr_t rhs = read_value(type->rhs(), ct, source);
                std::memcpy(&b[index * r_size], rhs->state(), r_size);
            }
            else {
                std::cout << "error reading function\n";
            }
        }
    }
    else if (match(source, "(")) {
        std::vector<state_space::ptr_t> types = type->subtypes();
        state_size_t offset = 1;
        state_size_t state = 0;

        // read block states
        for (state_space::ptr_t &t : types) {
            symbol::ptr_t val = read_value(t, ct, source);
            state += offset * val->index();
            offset += t->bits();
            match(source, ",");
        }
        if (match(source, ")")) {
            int size = std::min(sizeof(state), type_size);
            std::memcpy(b.get(), &state, size);
        }
        else {
            std::cout << "error reading subtypes\n";
        }
    }
    else {

        // read single values
        state_size state = type->state(source.front());
        int size = std::min(sizeof(state), type_size);
        std::memcpy(b.get(), &state, size);
        source.pop();
    }
    return std::make_shared<memory>(type, b.get());
}


symbol::ptr_t read_type_value(type_context &ct, tokens &source) {
    state_space::ptr_t type = read_type(ct, source);
    if (!match(source, ":")) {
        std::cout << "expected ':'\n";
    }
    return read_value(type, ct, source);
}


void read_func(program &p, type_context &ct, tokens &source) {
    if (match(source, "elem")) {
        std::string name = source.front();
        source.pop();
        p.add_symbol(name, read_type_value(ct, source));
    }
}


void read_loop(program &p, type_context &ct, tokens &source) {
    if (match(source, "proc")) {
        std::string name = source.front();
        source.pop();

        // type and value
        symbol::ptr_t init = read_type_value(ct, source);

        // function
        std::string func = source.front();
        source.pop();

        if (p.has_func(func)) {
            auto s = std::make_shared<stream>(init, p.get_func(func));
            p.add_stream(name, s);
        }
    }
}


program read_file(const std::string &fname) {
    tokens source = lex(fname);
    program p;
    type_context &ct = p;

    while (!source.empty()) {
        if (source.front() == "alias") {
            read_alias(ct, source);
        }
        else if (source.front() == "elem") {
            read_func(p, ct, source);
        }
        else if (source.front() == "proc") {
            read_loop(p, ct, source);
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
