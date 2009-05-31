// Numeric Adaptor Library

// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_DEFAULT_POLICY_HPP
#define NUMERIC_ADAPTOR_DEFAULT_POLICY_HPP

#include <sstream>
#include <iomanip>


namespace boost { namespace numeric_adaptor {


template <typename T>
struct default_policy
{
    typedef T value_type;

    // Default no initialization or pre-destruction is necessary
    static inline void init(T& value) {}
    static inline void destruct(T& value) {}

    // Use the default assignment
    static inline void copy(T const& source, T& dest) { dest = source; }

    // The default policy uses the default operators +, -, *, /
    static inline void add(value_type& r, const value_type& a, const value_type& b)
    { r = a + b; }
    static inline void add(value_type& a, const value_type& b)
    { a += b; }
    static inline void subtract(value_type& r, const value_type& a, const value_type& b)
    { r = a - b; }
    static inline void subtract(value_type& a, const value_type& b)
    { a -= b; }
    static inline void multiply(value_type& r, const value_type& a, const value_type& b)
    { r = a * b; }
    static inline void multiply(value_type& a, const value_type& b)
    { a *= b; }
    static inline void divide(value_type& r, const value_type& a, const value_type& b)
    { r = a / b; }
    static inline void divide(value_type& a, const value_type& b)
    { a /= b; }
    static inline void neg(value_type& r, const value_type& n)
    { r = -n; }

    // Default use the comparison operators
    static inline int compare(T const& a, T const& b)
    {
        return a < b ? -1 : a > b ? 1 : 0;
    }

    static inline std::string as_string(T const& a)
    {
        std::ostringstream out;
        out << std::setprecision(20) << a;
        return out.str();
    }
};


}} //  namespace boost::numeric_adaptor


#endif
