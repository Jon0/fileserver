#include <iostream>

#include <core/engine.h>

extern "C" {

void init(core::engine &e) {
    std::cout << "init\n";
}

void update() {
    std::cout << "update\n";
}

void recieve() {
    std::cout << "recieve\n";
}

}
