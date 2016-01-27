#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "type.h"

namespace core {


struct block {};


/**
 * function internal state
 */
struct state_map {
    std::string name;
    bool final_state;
    block write;

    // symbol -> state
    std::unordered_map<int, int> transitions;
};


/**
 * functions used for transforming input streams
 */
class function {
public:

    function(state_space::ptr_t type);

    bool is_valid();

    block eval(block in);

private:
    state_space::ptr_t type;

    // there could be many inputs
    function *input;

    state_map *current_state;

    std::unordered_map<int, state_map> states;
};


class program {
public:
    program();

    function *get_main();

    void add_function(const std::string &name, function &&f);

private:
    std::unordered_map<std::string, function> functions;

    // queues
    // {position -> data_type}
};



}
