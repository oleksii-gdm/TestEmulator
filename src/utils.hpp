#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <string>

bool isNameValid(const std::string& name)
{
    return name.length() <= 49;
}

bool isDigitString(const std::string& str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

#endif // UTILS_HPP
