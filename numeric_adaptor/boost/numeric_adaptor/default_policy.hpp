// Numeric Adaptor Library

// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_DEFAULT_POLICY_HPP_
#define NUMERIC_ADAPTOR_DEFAULT_POLICY_HPP_

#include <sstream>


namespace numeric_adaptor {


template <typename T>
struct default_policy
{
    typedef T type;

    // Default no initialization or pre-destruction is necessary
    static inline void init(T& value) {}
    static inline void destruct(T& value) {}

    // Use the default assignment
    static inline void copy(T const& source, T& dest) { dest = source; }

    // The default policy uses the default operators +, -, *, /
    static inline void add(type& r, const type& a, const type& b) { r = a + b; }
    static inline void subtract(type& r, const type& a, const type& b) { r = a - b; }
    static inline void multiply(type& r, const type& a, const type& b) { r = a * b; }
    static inline void divide(type& r, const type& a, const type& b) { r = a / b; }


    // Default use the comparison operators
    static inline int compare(T const& a, T const& b)
    {
        return a < b ? -1 : a > b ? 1 : 0;
    }

    static inline std::string as_string(T const& a)
    {
        std::ostringstream out;
        out << a;
        return out.str();
    }
};


} //  namespace numeric_adaptor


#endif
