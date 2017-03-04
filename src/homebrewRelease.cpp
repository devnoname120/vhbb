#include "homebrewRelease.h"


// Add type to yaml-cpp
namespace YAML {
bool convert<HomebrewRelease>::decode(const Node& node, HomebrewRelease& hbrel) {
  hbrel.date = node["date"].as<std::string>();
  hbrel.version = node["version"].as<std::string>();
  hbrel.eboot_hash = node["eboot_hash"].as<std::string>();

  hbrel.downloads = node["downloads"].as<HomebrewDownload>();
  return true;
}
};
