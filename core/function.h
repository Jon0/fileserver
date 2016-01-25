#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {


class alphabet {
public:
    alphabet(const std::string &name, std::vector<std::string> s);

    int size() const;
    int bits() const;
    std::string get_symbol(int index) const;

    const std::string name;
    const std::vector<std::string> symbols;
};


class state_space {
public:
    state_space(std::vector<alphabet> &a);

    const std::vector<alphabet> alphabets;
};

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
    void add_function(function &&f);

private:
    std::unordered_map<std::string, alphabet> alphabets;
    std::unordered_map<std::string, function> functions;

    // queues
    // {position -> data_type}
};



}
