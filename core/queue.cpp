#include "queue.h"

namespace core {

queue_region::queue_region() {}


queue::queue() {}


queue_region *queue::insert() {
    map.insert(std::make_pair(0, std::make_unique<queue_region>()));
    return map.at(0).get();
}

}
