#include <iostream>

#include <core/engine.h>

extern "C" {

void init(core::engine &e) {
    std::cout << "init\n";
}

void uninit(core::engine &e) {
    std::cout << "uninit\n";
}

void update() {
    std::cout << "update\n";
}

void recieve() {
    std::cout << "recieve\n";
}

}
