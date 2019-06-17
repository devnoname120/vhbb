#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include "date.h"


class Homebrew {
public:
	// Those should be const but then it's mandatory to use an initialization list
	std::string name;
	std::string icon; // FIXME Use special class instead
	std::string version;
	std::string author;
	std::string category;
	std::string type;
	std::string description;
	Date date;
	std::string titleid;
	std::string long_description;
	std::vector<std::string> screenshots;
	std::string url;
	std::string data;

	bool IsInstalled();
};


namespace YAML {
template<>
struct convert<Homebrew> {
  static bool decode(const Node& node, Homebrew& hb);
};
};
