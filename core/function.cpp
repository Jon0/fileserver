#include <cstring>

#include "function.h"

namespace core {


symbol::symbol(state_space::ptr_t value_type)
    :
    value_type(value_type) {}


const state_space::ptr_t symbol::type() const {
    return value_type;
}


memory::memory(state_space::ptr_t value_type, int element)
    :
    symbol(value_type) {
    block = std::make_unique<char[]>(value_type->bytes());
    int index = element % value_type->size();
    std::memcpy(block.get(), &index, value_type->bytes());
}


const char *memory::state() const {
    return block.get();
}


const symbol::ptr_t memory::eval(const symbol *in) const {
    int index = 0;
    std::memcpy(&index, in->state(), in->type()->bytes());
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
    index *= type()->rhs()->bytes();
    return std::make_shared<submemory>(type()->rhs(), &block[index]);
}


program::program() {}


symbol *program::get_main() {
    if (symbols.count("main") == 0) {
        return nullptr;
    }
    else {
        return symbols.at("main").get();
    }
}


void program::add_symbol(const std::string &name, symbol::ptr_t s) {
    symbols.insert(std::make_pair(name, s));
}

}
