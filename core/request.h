#pragma once

#include <memory>
#include <string>
#include <vector>

namespace core {

class request {
public:
    request();


private:
    request *goal;

    std::string constraint;

    std::vector<std::unique_ptr<request>> subr;

};


}
