#pragma once

#include <string>

template<typename... Args>
inline std::string StringFormat(const char* format, Args... args) {
    int s = std::snprintf(nullptr, 0, format, args...);
    std::string str{};
    str.resize(s + 1);
    std::snprintf(str.data(), str.size(), format, args...);
    return str;
}