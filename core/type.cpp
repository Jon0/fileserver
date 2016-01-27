#include "type.h"

namespace core {


const state_space::ptr_t state_space::empty_set = std::make_shared<state_enum>(std::vector<std::string>());


state_enum::state_enum(std::vector<std::string> s)
    :
    symbols(s) {}


int state_enum::size() const {
    return symbols.size();
}


int state_enum::bits() const {
    return size();
}


const state_space::ptr_t state_enum::lhs() const {
    return state_space::empty_set;
}


const state_space::ptr_t state_enum::rhs() const {
    return state_space::empty_set;
}


std::string state_enum::get_symbol(int index) const {
    return symbols.at(index % symbols.size());
}


state_multiply::state_multiply(std::vector<state_space::ptr_t> &sp)
    :
    subspaces(sp) {
}


int state_multiply::size() const {
    int total = 1;
    for (const state_space::ptr_t &s : subspaces) {
        total *= s->size();
    }
    return total;
}


int state_multiply::bits() const {
    return size();
}


const state_space::ptr_t state_multiply::lhs() const {
    return state_space::empty_set;
}


const state_space::ptr_t state_multiply::rhs() const {
    return state_space::empty_set;
}


state_function::state_function(state_space::ptr_t l, state_space::ptr_t r)
    :
    lspace(l),
    rspace(r) {}


int state_function::size() const {
    return lspace->size() * rspace->size();
}


int state_function::bits() const {
    return size();
}


const state_space::ptr_t state_function::lhs() const {
    return lspace;
}


const state_space::ptr_t state_function::rhs() const {
    return rspace;
}


type_context::type_context() {}


bool type_context::contains(const std::string &name) const {
    return types.count(name) > 0;
}


std::unordered_map<std::string, state_space::ptr_t> type_context::all_types() const {
    return types;
}


state_space::ptr_t type_context::get_type(const std::string &name) const {
    return types.at(name);
}


void type_context::use_type(const std::string &name, callback_t c) const {
    if (contains(name)) {
        c(get_type(name));
    }
}


void type_context::insert(const std::string &name, std::shared_ptr<state_space> s) {
    types.insert(std::make_pair(name, s));
}



}
