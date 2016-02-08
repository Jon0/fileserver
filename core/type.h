#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

using state_size_t = int;
using state_size = state_size_t;


class state_space {
public:
    using ptr_t = std::shared_ptr<state_space>;
    bool operator==(const state_space &other) const;
    int bytes() const;
    std::string desc() const;
    virtual state_size_t size() const = 0;
    virtual int bits() const = 0;
    virtual const ptr_t lhs() const = 0;
    virtual const ptr_t rhs() const = 0;
    virtual std::vector<state_space::ptr_t> subtypes() const = 0;
    virtual state_size_t state(const std::string &s) const = 0;
    virtual std::string symbol(state_size s) const = 0;
    static const ptr_t empty_set;
};


using alias_map_t = std::unordered_map<std::string, state_space::ptr_t>;



class state_enum : public state_space  {
public:
    state_enum(std::vector<std::string> s);

    state_size_t size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;
    std::vector<state_space::ptr_t> subtypes() const override;
    state_size_t state(const std::string &s) const override;
    std::string symbol(state_size s) const override;

private:
    const std::vector<std::string> symbols;

};


class state_multiply : public state_space {
public:
    state_multiply(const std::vector<state_space::ptr_t> &sp);

    state_size_t size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;
    std::vector<state_space::ptr_t> subtypes() const override;
    state_size_t state(const std::string &s) const override;
    std::string symbol(state_size s) const override;

private:
    const std::vector<state_space::ptr_t> subspaces;

};


class state_function : public state_space {
public:
    state_function(state_space::ptr_t l, state_space::ptr_t r);

    state_size_t size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;
    std::vector<state_space::ptr_t> subtypes() const override;
    state_size_t state(const std::string &s) const override;
    std::string symbol(state_size s) const override;

private:
    const state_space::ptr_t lspace;
    const state_space::ptr_t rspace;

};


class type_context {
public:
    using callback_t = std::function<void(state_space::ptr_t)>;

    type_context();

    bool contains(const std::string &name) const;
    alias_map_t all_types() const;
    state_space::ptr_t get_type(const std::string &name) const;
    void use_type(const std::string &name, callback_t c) const;
    void insert(const std::string &name, std::shared_ptr<state_space> s);

private:
    alias_map_t types;
};


}
