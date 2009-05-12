// Numeric Adaptor Library

// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_IEEE_POLICY_HPP_
#define NUMERIC_ADAPTOR_IEEE_POLICY_HPP_


#include <cmath>

#include <string>

#include <boost/numeric_adaptor/default_policy.hpp>


#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>


template <typename T>
struct ieee_policy : public default_policy<T>
{
    typedef T type;

    template <typename OtherType>
    static inline void set(type& value, const OtherType& v)   { value = boost::numeric_cast<T>(v); }

    static inline void set(type& value, const std::string& v) { value = boost::lexical_cast<T>(v); }

    static inline void sqrt(type& r, const type& a) { r = std::sqrt(a); }
    static inline void cos(type& r, const type& a) { r = std::cos(a); }
    static inline void sin(type& r, const type& a) { r = std::sin(a); }

    template <typename OtherType>
    static inline OtherType big_numeric_cast(const type& v)
    {
        return boost::numeric_cast<OtherType>(v);
    }
};


#endif
