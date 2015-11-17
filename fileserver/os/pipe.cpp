#include "pipe.h"

namespace fserv {
namespace os {

std::string exec(const std::string &cmd) {

	// TODO use file descriptors
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) return "ERROR";

	// copy output to a std::string
	char buffer[256];
	std::string result = "";

	while(!feof(pipe)) {
		if(fgets(buffer, 256, pipe) != NULL)
			result += buffer;
	}

	pclose(pipe);
	return result;
}

}
}
