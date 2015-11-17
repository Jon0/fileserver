#include <regex>

#include "template.h"

namespace fserv {
namespace io {


tmpl::tmpl(const std::string &in) {
	std::string s = in;
	std::smatch om;
	std::regex open("\\{{2}");
	std::regex close("\\}{2}");

	while (std::regex_search(s, om, open)) {
		std::smatch cm;
		std::string sf = om.suffix().str();
		if (std::regex_search(sf, cm, close)) {
			add_item(om.prefix().str(), false);
			add_item(cm.prefix().str(), true);
			s = cm.suffix().str();
		}
		else {
			break;
		}
	}
	add_item(s, false);
}


std::string tmpl::dump() const {
	std::string result = "";
	for (auto &i : items) {
		if (i.replace) {
			result += "{{" + i.content + "}}";
		}
		else {
			result += i.content;
		}
	}
	return result;
}


std::string tmpl::render(const strmap &map) const {
	std::string result = "";
	for (auto &i : items) {
		if (i.replace) {
			if (map.count(i.content) > 0) {
				result += map.at(i.content);
			}
		}
		else {
			result += i.content;
		}
	}
	return result;
}


std::string tmpl::render(std::function<std::string(const std::string &)> mapfn) const {
	std::string result = "";
	for (auto &i : items) {
		if (i.replace) {
			result += mapfn(i.content);
		}
		else {
			result += i.content;
		}
	}
	return result;
}


void tmpl::add_item(const std::string &content, bool replace) {
	if (!content.empty()) {
		items.emplace_back(tmpl_item{replace, content});
	}
}


}
}
