
#ifndef DEMANGLE_HPP
#define DEMANGLE_HPP

#include <string>
#include <typeinfo>
#include <cxxabi.h>

inline std::string
demangle(std::string const& name)
{
    std::size_t n = 2048;
    char buf[2048];
    abi::__cxa_demangle(name.c_str(), buf, &n, 0);
    return std::string(buf, ::strnlen(buf, n));
}

template <typename T>
inline std::string
demangle()
{
    return demangle(typeid(T).name());
}

#endif
