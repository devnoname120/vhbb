#include "homebrewInfo.h"

// Add type to yaml-cpp
namespace YAML
{
    Node convert<HomebrewInfo>::encode(const HomebrewInfo& info)
    {
        Node node;
        node["name"] = info.name;
        node["version"] = info.version;
        node["date"] = info.date;
        return node;
    }

    bool convert<HomebrewInfo>::decode(const Node& node, HomebrewInfo& info)
    {
        info.name = node["name"].as<std::string>();
        info.version = node["version"].as<std::string>();
        info.date = node["date"].as<Date>();

        return true;
    }
}; // namespace YAML