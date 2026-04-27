#pragma once

#include <string_view>
#include <utility>

template <auto T>
std::string_view extract_name()
{
    std::string_view name = __PRETTY_FUNCTION__;

    auto start = name.find("T = ") + 4;
    auto end = name.find_first_of(";]", start);
    std::string_view value = name.substr(start, end - start);

    auto colons = value.rfind("::");
    if (colons == std::string_view::npos)
        return {};
    return value.substr(colons + 2);
}

template <typename T, int... Is>
std::string_view get_enum(T e, std::integer_sequence<int, Is...>)
{
    std::string_view result;
    ((e == static_cast<T>(Is)
          ? (result = extract_name<static_cast<T>(Is)>(), true)
          : false) ||
     ...);
    return result;
}

template <int Max = 256, typename T>
std::string_view enum_name(T e)
{
    return get_enum(e, std::make_integer_sequence<int, Max>{});
}
