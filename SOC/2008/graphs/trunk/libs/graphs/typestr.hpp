
#ifndef DEMANGLE_HPP
#define DEMANGLE_HPP

#include <string>
#include <typeinfo>
#include <cxxabi.h>

template <typename T>
std::string
typestr()
{
    std::size_t n = 2048;
    char buf[2048];
    abi::__cxa_demangle(typeid(T).name(), buf, &n, 0);
    return std::string(buf, ::strlen(buf));
}

template <typename T>
inline std::string
typestr(T const&)
{ return typestr<T>(); }

#endif
