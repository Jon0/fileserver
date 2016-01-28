#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "type.h"

namespace core {


struct block {
    int size;
    char *state;
};


class symbol {
public:
    symbol(state_space::ptr_t value_type);

    const state_space::ptr_t type() const;
    virtual block state() const = 0;

private:
    const state_space::ptr_t value_type;

};

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
class function : public symbol {
public:
    function(state_space::ptr_t function_type);

    block state() const override;

    bool is_valid();

    block eval(block in);

private:
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
