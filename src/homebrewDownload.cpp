#include "HomebrewDownload.h"


// Add type to yaml-cpp
namespace YAML {
template<>
struct convert<HomebrewDownload> {
  static Node encode(const HomebrewDownload& hbdl) {
    Node node;
	// Don't need this
    return node;
  }

  static bool decode(const Node& node, HomebrewDownload& hbdl) {
    hbdl.hash = node["hash"].as<std::string>();
	  hbdl.url = node["url"].as<std::string>();
    return true;
  }
};
}
