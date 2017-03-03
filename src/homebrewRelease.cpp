#include "HomebrewRelease.h"


// Add type to yaml-cpp
namespace YAML {
template<>
struct convert<HomebrewRelease> {
  static Node encode(const HomebrewRelease& hbrel) {
    Node node;
	  // Don't need this
    return node;
  }

  static bool decode(const Node& node, HomebrewRelease& hbrel) {
    hbrel.date = node["date"].as<std::string>();
    hbrel.version = node["version"].as<std::string>();
    hbrel.eboot_hash = node["eboot_hash"].as<std::string>();

    hbrel.downloads = node["downloads"].as<HomebrewDownload>();
    return true;
  }
};
}