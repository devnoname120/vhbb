#pragma once

#include "date.h"

#include <yaml-cpp/yaml.h>

class HomebrewInfo
{
public:
    std::string name;
    std::string version;
    Date date;
};

namespace YAML
{
    template<> struct convert<HomebrewInfo>
    {
        static Node encode(const HomebrewInfo& info);
        static bool decode(const Node& node, HomebrewInfo& hb);
    };
}; // namespace YAML
