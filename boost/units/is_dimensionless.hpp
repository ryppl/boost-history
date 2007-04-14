// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_IS_DIMENSIONLESS_HPP
#define BOOST_UNITS_IS_DIMENSIONLESS_HPP

#include <boost/units/units_fwd.hpp>

namespace boost {

namespace units {

template<class T>
struct is_dimensionless :
    public mpl::false_
{ };

/// check if a unit is dimensionless
template<class System>
struct is_dimensionless< unit<dimensionless_type,System> > :
    public mpl::true_
{ };

/// check if a quantity is dimensionless
template<class Unit,class Y>
struct is_dimensionless< quantity<Unit,Y> > :
    public is_dimensionless<Unit>
{ };

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_IS_DIMENSIONLESS_HPP
