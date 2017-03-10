#include "homebrew.h"

#include "date.h"

class Date;

// Add type to yaml-cpp
namespace YAML {
bool convert<Homebrew>::decode(const Node& node, Homebrew& hb) {
  hb.name = node["name"].as<std::string>();
  hb.icon = node["icon"].as<std::string>();
  hb.version = node["version"].as<std::string>();
  hb.author = node["author"].as<std::string>();
  hb.category = node["type"].as<std::string>();
  hb.description = node["description"].as<std::string>();
  hb.date = node["date"].as<Date>();
  hb.titleid = node["titleid"].as<std::string>();

  return true;
}
};
