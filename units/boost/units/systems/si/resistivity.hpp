// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_RESISTIVITY_HPP
#define BOOST_UNITS_SI_RESISTIVITY_HPP

#include <boost/units/systems/si/base.hpp>

namespace boost {

namespace units { 

namespace SI {

/// resistivity : L^3 M T^-3 C^-2
typedef derived_dimension<length_dim,3,mass_dim,1,time_dim,-3,current_dim,-2>::type   resistivity_type;              

typedef unit<SI::resistivity_type,SI::system>                                           resistivity;

} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_RESISTIVITY_HPP
