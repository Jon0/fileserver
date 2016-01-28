#include "function.h"

namespace core {


symbol::symbol(state_space::ptr_t value_type)
    :
    value_type(value_type) {}


const state_space::ptr_t symbol::type() const {
    return value_type;
}


function::function(state_space::ptr_t function_type)
    :
    symbol(function_type) {
}


block function::state() const {

}


bool function::is_valid() {
    // check all input combinations reach an output
    int alphabet_size = type()->size();
    for (auto &s : states) {
        for (int i = 0; i < alphabet_size; ++i) {
            if (s.second.transitions.count(i) == 0) {
                return false;
            }
        }
    }
    return true;
}


block function::eval(block in) {
    state_map *current_state = &states.at(0);
    while (!current_state->final_state) {
        int read = 0; //input->eval(in);
        current_state = &states.at(read);
    }
    return current_state->write;
}


program::program() {}


function *program::get_main() {
    if (functions.count("main") == 0) {
        return nullptr;
    }
    else {
        return &functions.at("main");
    }
}


void program::add_function(const std::string &name, function &&f) {
    functions.insert(std::make_pair(name, f));
}

}
