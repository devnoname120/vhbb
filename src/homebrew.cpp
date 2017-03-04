#include "homebrew.h"


// Add type to yaml-cpp
namespace YAML {
bool convert<Homebrew>::decode(const Node& node, Homebrew& hb) {
  hb.title = node["title"].as<std::string>();
  hb.icon = node["icon"].as<std::string>();
  hb.author = node["author"].as<std::string>();
  hb.category = node["category"].as<std::string>();
  hb.description = node["description"].as<std::string>();
  
  hb.releases = node["releases"][0].as<HomebrewRelease>();
  return true;
}
};
