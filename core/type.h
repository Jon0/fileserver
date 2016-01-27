#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {


class state_space {
public:
    using ptr_t = std::shared_ptr<state_space>;
    virtual int size() const = 0;
    virtual int bits() const = 0;
    virtual const ptr_t lhs() const = 0;
    virtual const ptr_t rhs() const = 0;

    static const ptr_t empty_set;
};




class state_enum : public state_space  {
public:
    state_enum(std::vector<std::string> s);

    int size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;

    std::string get_symbol(int index) const;

private:
    const std::vector<std::string> symbols;

};


class state_multiply : public state_space {
public:
    state_multiply(std::vector<state_space::ptr_t> &sp);

    int size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;

private:
    const std::vector<state_space::ptr_t> subspaces;

};


class state_function : public state_space {
public:
    state_function(state_space::ptr_t l, state_space::ptr_t r);

    int size() const override;
    int bits() const override;
    const state_space::ptr_t lhs() const override;
    const state_space::ptr_t rhs() const override;

private:
    const state_space::ptr_t lspace;
    const state_space::ptr_t rspace;

};


class type_context {
public:
    using callback_t = std::function<void(state_space::ptr_t)>;

    type_context();

    bool contains(const std::string &name) const;
    std::unordered_map<std::string, state_space::ptr_t> all_types() const;
    state_space::ptr_t get_type(const std::string &name) const;
    void use_type(const std::string &name, callback_t c) const;

    void insert(const std::string &name, std::shared_ptr<state_space> s);

private:
    std::unordered_map<std::string, state_space::ptr_t> types;
};


}
