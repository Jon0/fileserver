#include <cstring>
#include <iostream>

#include "function.h"

namespace core {


symbol::symbol(state_space::ptr_t value_type)
    :
    value_type(value_type) {}


const state_space::ptr_t symbol::type() const {
    return value_type;
}


state_size symbol::index() const {
    auto t_size = static_cast<long unsigned int>(value_type->bytes());
    int copy_size = std::min(sizeof(state_size), t_size);
    state_size i = 0;
    std::memcpy(&i, state(), copy_size);
    i %= value_type->size();
    return i;
}


memory::memory(state_space::ptr_t value_type, state_size b)
    :
    symbol(value_type) {
    auto t_size = static_cast<long unsigned int>(value_type->bytes());
    int copy_size = std::min(sizeof(state_size), t_size);
    int size = type()->bytes();
    block = std::make_unique<char[]>(size);
    b %= value_type->size();
    std::memcpy(block.get(), &b, copy_size);
}


memory::memory(state_space::ptr_t value_type, const char *b)
    :
    symbol(value_type) {
    int size = type()->bytes();
    block = std::make_unique<char[]>(size);
    std::memcpy(block.get(), b, size);
}


memory::memory(symbol *a, symbol *b)
    :
    symbol(type_mix(a, b)) {
    block = std::make_unique<char[]>(type()->bytes());
    std::memcpy(&block[0], a->state(), a->type()->bytes());
    std::memcpy(&block[a->type()->bytes()], b->state(), b->type()->bytes());
}


const char *memory::state() const {
    return block.get();
}


const symbol::ptr_t memory::eval(const symbol *in) const {
    int index = 0;
    std::memcpy(&index, in->state(), in->type()->bytes());
    index %= type()->lhs()->size();
    index *= type()->rhs()->bytes();
    return std::make_shared<submemory>(type()->rhs(), &block[index]);
}


submemory::submemory(state_space::ptr_t value_type, const char *mem)
    :
    symbol(value_type),
    block(mem) {
}


const char *submemory::state() const {
    return block;
}


const symbol::ptr_t submemory::eval(const symbol *in) const {
    int index = 0;
    std::memcpy(&index, in->state(), in->type()->bytes());
    index %= type()->lhs()->size();
    index *= type()->rhs()->bytes();
    return std::make_shared<submemory>(type()->rhs(), &block[index]);
}


stream::stream(symbol::ptr_t initial_state, symbol::ptr_t transition)
    :
    state(initial_state),
    function(transition) {}


void stream::print_state() const {
    std::cout << "state: " << std::to_string(state->index()) << "\n";
}


void stream::process(symbol::ptr_t input) {
    auto m = std::make_shared<memory>(state.get(), input.get());
    auto r = function->eval(m.get());
    if (state->type() == r->type()) {
        state = r;
    }
}


void stream::test_input() {
    auto random = std::make_shared<memory>(state->type(), rand());
    std::cout << "input: " << std::to_string(random->index()) << "\n";
    process(random);
}


program::program() {}


bool program::has_func(const std::string &name) const {
    return symbols.count(name) > 0;
}


stream::ptr_t program::get_main() {
    return get_stream("main");
}


symbol::ptr_t program::get_func(const std::string &name) {
    if (symbols.count(name) == 0) {
        return nullptr;
    }
    else {
        return symbols.at(name);
    }
}


stream::ptr_t program::get_stream(const std::string &name) {
    if (streams.count(name) == 0) {
        return nullptr;
    }
    else {
        return streams.at(name);
    }
}


void program::add_symbol(const std::string &name, symbol::ptr_t s) {
    symbols.insert(std::make_pair(name, s));
}


void program::add_stream(const std::string &name, stream::ptr_t s) {
    streams.insert(std::make_pair(name, s));
}


state_space::ptr_t type_mix(symbol *a, symbol *b) {
    std::vector<state_space::ptr_t> types({a->type(), b->type()});
    return std::make_shared<state_multiply>(types);
}


}
