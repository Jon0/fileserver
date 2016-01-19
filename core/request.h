#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "channel.h"

namespace core {


class request {
public:
    request();

    void find_channels();


private:
    endpoint *input;

    request *goal;

    std::string constraint;

    std::vector<std::unique_ptr<request>> subr;

};


}
