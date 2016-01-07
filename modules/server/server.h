#pragma once

#include <iostream>
#include <string>
#include <core/engine.h>

extern "C" {

void init(core::engine &e);

}

namespace os {

void sockaccept(core::engine &e);

void sockctl(core::channel &c);

void sendtcp(core::channel &c);

}
