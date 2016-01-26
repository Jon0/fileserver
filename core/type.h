#pragma once

#include <string>
#include <vector>

namespace core {


class state_space {
public:
    virtual int size() const = 0;
    virtual int bits() const = 0;
    virtual state_space *lhs() = 0;
    virtual state_space *rhs() = 0;
};


class state_enum : public state_space  {
public:
    state_enum(std::vector<std::string> s);

    int size() const override;
    int bits() const override;
    state_space *lhs() override;
    state_space *rhs() override;

    std::string get_symbol(int index) const;

private:
    const std::vector<std::string> symbols;

};



class state_multiply : public state_space {
public:
    state_multiply(std::vector<state_space *> &sp);

    int size() const override;
    int bits() const override;

private:
    const std::vector<state_space *> subspaces;

};



class type {
public:
    type();
};


}
