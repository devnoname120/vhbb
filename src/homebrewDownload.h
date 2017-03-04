#pragma once

#include <global_include.h>

class HomebrewDownload {
public:
  std::string hash;
	std::string url;
};

namespace YAML {
template<>
struct convert<HomebrewDownload> {
  static bool decode(const Node& node, HomebrewDownload& hbdl);
};
};
