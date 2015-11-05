#pragma once

#include <memory>
#include <vector>

namespace fserv {
namespace io {

class buffer {
public:
	buffer();


private:
	int current_size;

	std::vector<std::unique_ptr<char[]>> alloc_stack;

};

}
}
