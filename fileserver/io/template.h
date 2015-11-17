#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace fserv {
namespace io {

using strmap = std::unordered_map<std::string, std::string>;

struct tmpl_item {
	bool replace;
	std::string content;
};

class tmpl {
public:
	tmpl(const std::string &in);

	std::string dump() const;

	std::string render(const strmap &map) const;

	std::string render(std::function<std::string(const std::string &)> mapfn) const;

private:
	void add_item(const std::string &content, bool replace);

	std::vector<tmpl_item> items;


};

}
}
