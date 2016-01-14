#include "request.h"

namespace core {


function::function() {

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
