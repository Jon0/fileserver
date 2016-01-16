#include "request.h"

namespace core {


function::function() {
    in_type = {
        "binary",
        {"0", "1"}
    };
}


bool function::is_valid() {
    // check all input combinations reach an output
    int alphabet_size = in_type.symbols.size();
    for (auto &s : states) {
        for (int i = 0; i < alphabet_size; ++i) {
            if (s.second.transitions.count(i) == 0) {
                return false;
            }
        }
    }
    return true;
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


void request::find_channels() {
    // connect 1 input to 1 output
}


}
