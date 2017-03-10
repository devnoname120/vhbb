#include "homebrew.h"


// Add type to yaml-cpp
namespace YAML {
bool convert<Homebrew>::decode(const Node& node, Homebrew& hb) {
  hb.name = node["name"].as<std::string>();
  hb.icon = node["icon"].as<std::string>();
  hb.version = node["version"].as<std::string>();
  hb.author = node["author"].as<std::string>();
  hb.category = node["type"].as<std::string>();
  hb.description = node["description"].as<std::string>();
  hb.date = node["date"].as<std::string>();
  hb.titleid = node["titleid"].as<std::string>();

  return true;
}
};
