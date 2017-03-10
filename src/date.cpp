#include "date.h"

#include <global_include.h>

Date::Date()
{
    
}

Date::Date(const std::string &date)
{
    //TODO
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
    std::string date_str = node.as<std::string>();
    dbg_printf(DBG_DEBUG, "Date: %s", date_str.c_str());
    date.year = atoi(date_str.substr(0, 4).c_str());
    dbg_printf(DBG_DEBUG, "Year: %d", date.year);
    date.month = atoi(date_str.substr(5, 2).c_str());
    dbg_printf(DBG_DEBUG, "Month: %d", date.month);
    date.day = atoi(date_str.substr(8, 2).c_str());
    dbg_printf(DBG_DEBUG, "Day: %d", date.day);
    return true;
}
};