#pragma once

#include <string>
#include <vector>
#include <queue>

#include "function.h"

namespace core {

using tokens = std::queue<std::string>;

bool match(tokens &source, const std::string &t);



state_space::ptr_t read_type(type_context &ct, tokens &source);
void read_alias(type_context &ct, tokens &source);
void read_mapping(type_context &ct, tokens &source);
symbol::ptr_t read_value(state_space::ptr_t type, type_context &ct, tokens &source);
symbol::ptr_t read_type_value(type_context &ct, tokens &source);
void read_func(program &p, type_context &ct, tokens &source);
void read_loop(program &p, type_context &ct, tokens &source);

program read_file(const std::string &fname);

tokens lex(const std::string &fname);

}
