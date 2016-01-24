#pragma once

#include <string>
#include <vector>
#include <queue>

#include "function.h"

namespace core {

using tokens = std::queue<std::string>;

bool match(tokens &source, const std::string &t);

alphabet read_enum(tokens &source);

function read_func(tokens &source);

program read_file(const std::string &fname);

tokens lex(const std::string &fname);

}
