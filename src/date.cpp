#include "date.h"

#include <utility>

Date::Date() = default;

bool IsNewer::operator()(const Homebrew& hb1, const Homebrew& hb2) const
{
    bool res = true;
    if (hb1.date != hb2.date)
    {
        res = hb1.date < hb2.date;
    }

    return dsc_ ? !res : res;
}

namespace YAML
{
    Node convert<Date>::encode(const Date& date)
    {
        char buf[11] = { 0 };
        snprintf(buf, 11, "%04u-%02u-%02u", date.year, date.month, date.day);

        return convert<std::string>::encode(std::string(buf));
    }

    bool convert<Date>::decode(const Node& node, Date& date)
    {
        date.str = node.as<std::string>();
        date.year = std::atoi(date.str.substr(0, 4).c_str());
        date.month = std::atoi(date.str.substr(5, 2).c_str());
        date.day = std::atoi(date.str.substr(8, 2).c_str());
        return true;
    }
}; // namespace YAML
bool operator<(const Date& a, const Date& b)
{
    bool res = true;
    if (a.year < b.year)
    {
        res = true;
    }
    else if (a.year > b.year)
    {
        res = false;
    }
    else if (a.year == b.year)
    {
        if (a.month < b.month)
        {
            res = true;
        }
        else if (a.month > b.month)
        {
            res = false;
        }
        else if (a.month == b.month)
        {
            if (a.day < b.day)
            {
                res = true;
            }
            else if (a.day > b.day)
            {
                res = false;
            }
            else if (a.day == b.day)
            {
                res = true; // Default behavior if equal
            }
        }
    }
    return res;
}

bool operator>(const Date& a, const Date& b)
{
    if (a == b)
    {
        return false;
    }

    return !(a < b);
}

bool operator==(const Date& a, const Date& b)
{
    return a.year == b.year && a.month == b.month && a.day == b.day;
}

bool operator!=(const Date& a, const Date& b)
{
    return !(a == b);
}
Date::Date(std::string date)
{
    str = std::move(date);
    year = std::atoi(str.substr(0, 4).c_str());
    month = std::atoi(str.substr(5, 2).c_str());
    day = std::atoi(str.substr(8, 2).c_str());
}
