#include <cmath>

#include "type.h"

namespace core {


const state_space::ptr_t state_space::empty_set = std::make_shared<state_enum>(std::vector<std::string>());


bool state_space::operator==(const state_space &other) const {
    return size() == other.size();
}


int state_space::bytes() const {
    return std::ceil(bits() / 8.0);
}


std::string state_space::desc() const {
    std::string result = "";
    result += "(" + std::to_string(lhs()->size());
    result += ") -> (" + std::to_string(rhs()->size()) + ")";
    return result;
}


state_enum::state_enum(std::vector<std::string> s)
    :
    symbols(s) {}


state_size_t state_enum::size() const {
    if (symbols.empty()) {
        return 1;
    }
    return symbols.size();
}


int state_enum::bits() const {
    if (symbols.empty()) {
        return 0;
    }
    return std::ceil(std::log2(size()));
}


const state_space::ptr_t state_enum::lhs() const {
    return state_space::empty_set;
}


const state_space::ptr_t state_enum::rhs() const {
    return std::make_shared<state_enum>(symbols);
}


std::vector<state_space::ptr_t> state_enum::subtypes() const {
    return std::vector<state_space::ptr_t>({rhs()});
}


state_size_t state_enum::state(const std::string &s) const {
    for (state_size_t i = 0; i < symbols.size(); ++i) {
        if (symbols.at(i) == s) {
            return i;
        }
    }
    return 0;
}


std::string state_enum::symbol(state_size s) const {
    if (symbols.empty()) {
        return "emptyset";
    }
    return symbols.at(s % symbols.size());
}


state_multiply::state_multiply(const std::vector<state_space::ptr_t> &sp)
    :
    subspaces(sp) {
}


state_size_t state_multiply::size() const {
    int total = 1;
    for (const state_space::ptr_t &s : subspaces) {
        total *= s->size();
    }
    return total;
}


int state_multiply::bits() const {
    int total = 0;
    for (const state_space::ptr_t &s : subspaces) {
        total += s->bits();
    }
    return total;
}


const state_space::ptr_t state_multiply::lhs() const {
    return state_space::empty_set;
}


const state_space::ptr_t state_multiply::rhs() const {
    return std::make_shared<state_multiply>(subspaces);
}


std::vector<state_space::ptr_t> state_multiply::subtypes() const {
    return subspaces;
}


state_size_t state_multiply::state(const std::string &s) const {
    return 0;
}


std::string state_multiply::symbol(state_size s) const {
    return "";
}


state_function::state_function(state_space::ptr_t l, state_space::ptr_t r)
    :
    lspace(l),
    rspace(r) {}


state_size_t state_function::size() const {
    return  std::pow(rspace->size(), lspace->size());
}


int state_function::bits() const {
    return lspace->size() * rspace->bits();
}


const state_space::ptr_t state_function::lhs() const {
    return lspace;
}


const state_space::ptr_t state_function::rhs() const {
    return rspace;
}


std::vector<state_space::ptr_t> state_function::subtypes() const {
    state_space::ptr_t ptr = std::make_shared<state_function>(lspace, rspace);
    return std::vector<state_space::ptr_t>({ptr});
}


state_size_t state_function::state(const std::string &s) const {
    return 0;
}


std::string state_function::symbol(state_size s) const {
    return "";
}


type_context::type_context() {}


bool type_context::contains(const std::string &name) const {
    return types.count(name) > 0;
}


alias_map_t type_context::all_types() const {
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
