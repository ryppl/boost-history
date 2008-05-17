// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_KILOGRAM_BASE_UNIT_HPP
#define BOOST_UNITS_SI_KILOGRAM_BASE_UNIT_HPP

#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/systems/cgs/base_units/gram.hpp>

namespace boost {

namespace units {

namespace SI {

typedef scaled_base_unit<boost::units::CGS::gram_base_unit, scale<10, static_rational<3> > > kilogram_base_unit;

} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_KILOGRAM_BASE_UNIT_HPP
