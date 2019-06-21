#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

class Date
{
public:
    Date();
    Date(std::string date); // NOLINT(google-explicit-constructor)

    std::string str;

    int year;
    int month;
    int day;

    friend bool operator<(const Date& a, const Date& b);
    friend bool operator>(const Date& a, const Date& b);
    friend bool operator==(const Date& a, const Date& b);
    friend bool operator!=(const Date& a, const Date& b);
};

class Homebrew;

#include "homebrew.h"

struct IsNewer : public std::unary_function<bool, bool>
{
    IsNewer(bool dsc)
        : dsc_(dsc)
    {
    }
    bool operator()(const Homebrew& hb1, const Homebrew& hb2) const;

    bool dsc_;
};

namespace YAML
{
    template<> struct convert<Date>
    {
        static Node encode(const Date& date);
        static bool decode(const Node& node, Date& date);
    };
};