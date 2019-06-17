#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

class Date {
public:
    Date();

	std::string str;

    int year;
    int month;
    int day;
};

class Homebrew;

#include "homebrew.h"


struct IsNewer : public std::unary_function<bool, bool> {
    IsNewer(bool dsc) : dsc_(dsc) {}
    bool operator()(const Homebrew &hb1, const Homebrew &hb2) const;

    bool dsc_;
};

namespace YAML {
template<>
struct convert<Date> {
  static bool decode(const Node& node, Date& date);
};
};