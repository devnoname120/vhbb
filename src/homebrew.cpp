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
  if (node["long_description"])
    hb.long_description = node["long_description"].as<std::string>();
  if (node["screenshots"])
    hb.screenshots = node["screenshots"].as<std::vector<std::string>>();

  hb.url = node["url"].as<std::string>();
  if (node["data"])
    hb.data = node["data"].as<std::string>();

  return true;
}
};
