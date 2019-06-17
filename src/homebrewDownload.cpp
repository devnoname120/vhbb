#include "homebrewDownload.h"

// Add type to yaml-cpp
namespace YAML {
bool convert<HomebrewDownload>::decode(const Node& node, HomebrewDownload& hbdl) {
  hbdl.hash = node["pair_hash"].as<std::string>();
  hbdl.url = node["url"].as<std::string>();
  return true;
}
}
