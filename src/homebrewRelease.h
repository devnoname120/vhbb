#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include "homebrewDownload.h"

class HomebrewRelease {
public:
    std::string date;
	std::string version;
	std::string eboot_hash;

	HomebrewDownload downloads;
};

namespace YAML {
template<>
struct convert<HomebrewRelease> {
  static bool decode(const Node& node, HomebrewRelease& hbrel);
};
};
