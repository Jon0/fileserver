#pragma once

#include <memory>
#include <unordered_map>

namespace core {

enum class queue_state {
    empty,
    ready
};

class queue_region {
public:
    queue_region();

private:
    queue_state state;
    int type;
    int size;

};


class queue {
public:
    queue();

    queue_region *insert();

private:
    using region_ptr_t = std::unique_ptr<queue_region>;
    using region_map_t = std::unordered_map<int, region_ptr_t>;

    region_map_t map;
};


}
