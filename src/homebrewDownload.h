#pragma once

#include "yaml-cpp/yaml.h"
#include <string>

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
