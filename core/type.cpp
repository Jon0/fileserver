#include "type.h"

namespace core {


state_enum::state_enum(std::vector<std::string> s)
    :
    symbols(s) {}


int state_enum::size() const {
    return symbols.size();
}


int state_enum::bits() const {
    return size();
}


state_space * state_enum::lhs() {
    return this;
}


state_space * state_enum::rhs() {
    return this;
}


std::string state_enum::get_symbol(int index) const {
    return symbols.at(index % symbols.size());
}


state_multiply::state_multiply(std::vector<state_space *> &sp)
    :
    subspaces(sp) {
}

int state_multiply::size() const {
    int total = 1;
    for (state_space *s : subspaces) {
        total *= s->size();
    }
    return total;
}

int state_multiply::bits() const {
    return size();    
}

type::type() {

}


}
