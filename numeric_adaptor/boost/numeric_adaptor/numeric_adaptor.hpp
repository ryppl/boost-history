// Numeric Adaptor Library
//
// Copyright Bruno Lalande 2009
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP
#define BOOST_NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP


#define NUMERIC_ADAPTOR_INCLUDED


#include <cmath>
#include <boost/static_assert.hpp>
#include <boost/math/special_functions/hypot.hpp>
#include <boost/lexical_cast.hpp>


namespace boost {


namespace numeric_adaptor {


template <class Target, class Source>
struct to_traits
{
    static Target apply(const Source& source)
    { return boost::lexical_cast<Target>(source); }
};


} // namespace numeric_adaptor


template <class T>
T abs(T value)
{ return std::abs(value); }

template <class T>
T sqrt(T value)
{ return std::sqrt(value); }

template <class T>
T cos(T value)
{ return std::cos(value); }

template <class T>
T sin(T value)
{ return std::sin(value); }

template <class T>
T tan(T value)
{ return std::tan(value); }

template <class T>
T atan(T value)
{ return std::atan(value); }

template <class T>
T hypot(T a, T b)
{ return boost::math::hypot(a, b); }

template <class Target, class Source>
Target to(const Source& source)
{ return numeric_adaptor::to_traits<Target, Source>::apply(source); }


} // namespace boost


#endif // BOOST_NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP
