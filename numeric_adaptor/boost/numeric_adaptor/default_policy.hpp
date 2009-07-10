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


template <typename Derived, typename T>
struct default_policy
{
    typedef T value_type;

    default_policy()
    {}

    // Use the default assignment
    default_policy(default_policy const& source):
    value(source.value)
    {}

    // The default policy uses the default operators +, -, *, /
    static inline void add(default_policy& r, const default_policy& a, const default_policy& b)
    { r.value = a.value + b.value; }
    static inline void add(default_policy& a, const default_policy& b)
    { a.value += b.value; }
    static inline void subtract(default_policy& r, const default_policy& a, const default_policy& b)
    { r.value = a.value - b.value; }
    static inline void subtract(default_policy& a, const default_policy& b)
    { a.value -= b.value; }
    static inline void multiply(default_policy& r, const default_policy& a, const default_policy& b)
    { r.value = a.value * b.value; }
    static inline void multiply(default_policy& a, const default_policy& b)
    { a.value *= b.value; }
    static inline void divide(default_policy& r, const default_policy& a, const default_policy& b)
    { r.value = a.value / b.value; }
    static inline void divide(default_policy& a, const default_policy& b)
    { a.value /= b.value; }
    static inline void neg(default_policy& r, const default_policy& n)
    { r.value = -n.value; }

    static inline void cos(default_policy& r, default_policy const& a)
    {
        double d = Derived::template big_numeric_cast<double>(static_cast<const Derived&>(a));
        Derived::set(r.value, std::cos(d));
    }

    static inline void sin(default_policy& r, default_policy const& a)
    {
        double d = Derived::template big_numeric_cast<double>(static_cast<const Derived&>(a));
        Derived::set(r.value, std::sin(d));
    }

    static inline void tan(default_policy& r, default_policy const& a)
    {
        double d = Derived::template big_numeric_cast<double>(static_cast<const Derived&>(a));
        Derived::set(r.value, std::tan(d));
    }

    static inline void atan(default_policy& r, default_policy const& a)
    {
        double d = Derived::template big_numeric_cast<double>(static_cast<const Derived&>(a));
        Derived::set(r.value, std::atan(d));
    }

    static inline void sqrt(default_policy& r, default_policy const& a)
    {
        double d = Derived::template big_numeric_cast<double>(static_cast<const Derived&>(a));
        Derived::set(r.value, std::sqrt(d));
    }

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

protected:
    value_type value;
};


}} //  namespace boost::numeric_adaptor


#endif
