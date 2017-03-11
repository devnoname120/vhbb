#include "date.h"

#include <global_include.h>

Date::Date()
{

}

Date::Date(std::string date)
{
    return;
}

bool IsNewer::operator()(const Homebrew &hb1, const Homebrew &hb2) const {
    bool res = true;
    if (hb1.date.year < hb2.date.year) {
        res = true;
    } else if (hb1.date.year > hb2.date.year) {
        res = false;
    } else if (hb1.date.year == hb2.date.year) {
        if (hb1.date.month < hb2.date.month) {
            res = true;
        } else if (hb1.date.month > hb2.date.month) {
            res = false;
        } else if (hb1.date.month == hb2.date.month) {
            if (hb1.date.day < hb2.date.day) {
                res = true;
            } else if (hb1.date.day > hb2.date.day) {
                res = false;
            } else if (hb1.date.day == hb2.date.day) {
                res = true; // Default behavior if equal
            }
        }
    }

    return dsc_? !res : res;
}

namespace YAML {
bool convert<Date>::decode(const Node& node, Date& date) {
    date.str = node.as<std::string>();
    date.year = atoi(date.str.substr(0, 4).c_str());
    date.month = atoi(date.str.substr(5, 2).c_str());
    date.day = atoi(date.str.substr(8, 2).c_str());
    return true;
}
};