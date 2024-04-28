//
// Created by HITO on 28/04/24.
//

#ifndef GIN_FORMATTER_HPP
#define GIN_FORMATTER_HPP

#include <string>
#include <string_view>

namespace Gin::Utils {

    template<typename T>
    inline std::string Format(T v) {
        static_assert(false, "Formatter not implemented.");
    }

    template<>
    inline std::string Format<std::string>(std::string v) {
        return v;
    }

    template<>
    inline std::string Format<std::string_view>(std::string_view v) {
        return v.data();
    }

    template<>
    inline std::string Format<const char*>(const char* v) {
        return v;
    }

    template<>
    inline std::string Format<int>(int v) {
        return std::to_string(v);
    }

}

#endif //GIN_FORMATTER_HPP
