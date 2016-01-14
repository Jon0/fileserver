#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

/**
 * function internal state
 */
struct state {
    int write;
    std::unordered_map<int, int> transitions;
};


/**
 * functions used for transforming input streams
 */
class function {
public:
    function();

    int compute();

private:

    // there could be many inputs
    function *input;

    state *current_state;

    std::unordered_map<int, state> states;
};


class request {
public:
    request();


private:
    request *goal;

    std::string constraint;

    std::vector<std::unique_ptr<request>> subr;

};


}
