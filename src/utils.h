#define _countof(a) (sizeof(a)/sizeof(*(a)))

#include <string>
#include <vector>


inline double lerpd(double value, double from_max, double to_max)
{
    return value*to_max/from_max;
}


template <typename T, typename V>
T align_center(T a, V b)
{
    return (a - b) / 2;
}

template <typename T, typename V>
T align_left(T a, V b)
{
    return a - b;
}

std::string join_strings(const std::vector<std::string>& v, char c);
std::vector<std::string> split_string(const std::string &s, char delim=' ');
