#include "object.h"

namespace core {


object::object(const std::string &data)
	:
	data(data) {
}


std::string object::str() const {
	return data;
}


}
