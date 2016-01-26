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
    function();

    std::string name() const;

    bool is_valid();

    block eval(block in);



private:
    std::string fn_name;
    state_enum in_type;

    // there could be many inputs
    function *input;

    state_map *current_state;

    std::unordered_map<int, state_map> states;
};


struct alias {
    const std::string name;
    const state_enum type;
};


class program {
public:
    program();

    function *get_main();

    void add_alias(alias &&a);
    void add_function(function &&f);

private:
    std::unordered_map<std::string, state_enum> alphabets;
    std::unordered_map<std::string, function> functions;

    // queues
    // {position -> data_type}
};



}
