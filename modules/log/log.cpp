#include <iostream>

#include <core/engine.h>

extern "C" {

void log_init(core::engine &e) {
    std::cout << "log init\n";
}

void log_uninit(core::engine &e) {
    std::cout << "log uninit\n";
}

void update() {
    std::cout << "update\n";
}

void recieve() {
    std::cout << "recieve\n";
}

}
