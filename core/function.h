#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

struct block {};


struct alphabet {
    std::string name;
    std::vector<std::string> symbols;
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
class function {
public:
    function();

    bool is_valid();

    block eval(block in);



private:
    alphabet in_type;

    // there could be many inputs
    function *input;

    state_map *current_state;

    std::unordered_map<int, state_map> states;
};


class program {
public:
    program();

    function *get_main();

    void add_alphabet(alphabet &&a);

private:
    std::unordered_map<std::string, alphabet> alphabets;
    std::unordered_map<std::string, function> functions;

    // queues
    // {position -> data_type}
};



}
