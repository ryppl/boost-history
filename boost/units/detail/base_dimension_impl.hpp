// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_BASE_DIMENSION_IMPL_HPP
#define BOOST_UNITS_BASE_DIMENSION_IMPL_HPP

#include <boost/mpl/long.hpp>

namespace boost {

namespace units {

namespace detail {

typedef char no;
struct yes { no dummy[2]; };

template<bool> struct ordinal_has_already_been_defined;

template<>
struct ordinal_has_already_been_defined<true>   { };

template<>
struct ordinal_has_already_been_defined<false>  { typedef void type; };

}

/// This must be in namespace boost::units so that ADL
/// will work with friend functions defined inline.
/// INTERNAL ONLY
template<long N> struct long_ { };

/// Again this needs to be in the same namespace as long_
/// we need a mangled name because it must be found by ADL
/// INTERNAL ONLY
template<class T>
detail::no 
boost_units_prevent_redefinition(const T&) 
{ return(detail::no()); }

/// Ditto.
/// INTERNAL ONLY
template<class T, long N> struct base_dimension_pair { };

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_BASE_DIMENSION_IMPL_HPP
