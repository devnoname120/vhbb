#pragma once

#include <global_include.h>

#include "homebrewRelease.h"

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
	std::string date;
	std::string titleid;
};

namespace YAML {
template<>
struct convert<Homebrew> {
  static bool decode(const Node& node, Homebrew& hb);
};
};
