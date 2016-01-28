#include "queue.h"

namespace core {


queue_region::queue_region() {}


queue::queue()
    :
    front_id(0),
    back_id(0) {}


bool queue::empty() const {
    return map.empty();
}


queue_region *queue::front() {
    return map.at(front_id).get();
}


queue_region *queue::insert() {
    int addr = back_id++;
    map.insert(std::make_pair(addr, std::make_shared<queue_region>()));
    return map.at(addr).get();
}


}
