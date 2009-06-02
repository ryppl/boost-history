// Numeric Adaptor Library

// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_IEEE_POLICY_HPP
#define NUMERIC_ADAPTOR_IEEE_POLICY_HPP


#include <cmath>

#include <string>

#include <boost/numeric_adaptor/default_policy.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/math/special_functions/hypot.hpp>


namespace boost { namespace numeric_adaptor {


template <typename T>
struct ieee_policy : public default_policy<T>
{
    typedef T value_type;

    template <typename OtherType>
    static inline void set(value_type& value, OtherType const& v)   { value = v; } //boost::numeric_cast<T>(v); }

    static inline void set(value_type& value, std::string const& v) { value = boost::lexical_cast<T>(v); }

    static inline void sqrt(value_type& r, value_type const& a) { r = std::sqrt(a); }
    static inline void cos(value_type& r, value_type const& a) { r = std::cos(a); }
    static inline void sin(value_type& r, value_type const& a) { r = std::sin(a); }
    static inline void tan(value_type& r, value_type const& a) { r = std::tan(a); }
    static inline void atan(value_type& r, value_type const& a) { r = std::atan(a); }
    static inline void abs(value_type& r, value_type const& a) { r = std::abs(a); }
    static inline void hypot(value_type& r, value_type const& a, value_type const& b)
    {
        r = boost::math::hypot(a, b);
    }

    template <typename OtherType>
    static inline OtherType big_numeric_cast(value_type const& v)
    {
        return boost::numeric_cast<OtherType>(v);
    }
};


}} // namespace boost::numeric_adaptor


#endif
