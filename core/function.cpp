#include "function.h"

namespace core {


function::function(state_space::ptr_t type)
    :
    type(type) {
}


bool function::is_valid() {
    // check all input combinations reach an output
    int alphabet_size = type->size();
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
    current_state = &states.at(0);
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
