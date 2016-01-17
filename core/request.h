#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "channel.h"

namespace core {


struct alphabet {
    std::string name;
    std::vector<std::string> symbols;
};


/**
 * function internal state
 */
struct state_map {
    int write;
    std::string name;

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

    int compute();



private:
    alphabet in_type;

    // there could be many inputs
    function *input;

    state_map *current_state;

    std::unordered_map<int, state_map> states;
};


class request {
public:
    request();

    void find_channels();


private:
    endpoint *input;

    request *goal;

    std::string constraint;

    std::vector<std::unique_ptr<request>> subr;

};


}
