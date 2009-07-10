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
struct ieee_policy : public default_policy<ieee_policy<T>, T>
{
    typedef T value_type;

    template <typename OtherType>
    static inline void set(ieee_policy& p, OtherType const& v)
    { p.value = v; } //boost::numeric_cast<T>(v); }

    static inline void set(ieee_policy& p, std::string const& v)
    { p.value = boost::lexical_cast<T>(v); }

    static inline void abs(ieee_policy& r, ieee_policy const& a)
    { r.value = std::abs(a.value); }

    static inline void hypot(ieee_policy& r, ieee_policy const& a, ieee_policy const& b)
    {
        r.value = boost::math::hypot(a.value, b.value);
    }

    template <typename OtherType>
    static inline OtherType big_numeric_cast(ieee_policy const& v)
    {
        return boost::numeric_cast<OtherType>(v.value);
    }
};


}} // namespace boost::numeric_adaptor


#endif
