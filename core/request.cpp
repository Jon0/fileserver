#include "request.h"

namespace core {


function::function() {
    in_type = {
        "binary",
        {"0", "1"}
    };
}


bool function::is_valid() {
    // todo: check all input combinations
    // reach an output

    return false;
}

int function::compute() {
    current_state = &states.at(0);
    while (!current_state->write) {
        int read = input->compute();
        current_state = &states.at(read);
    }
    return current_state->write;
}


request::request() {

}

}
