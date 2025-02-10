#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

bool isNameValid(const std::string& name)
{
    return name.length() <= 49;
}

#endif // UTILS_HPP
