// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_IMPEDANCE_HPP
#define BOOST_UNITS_SI_IMPEDANCE_HPP

#include <boost/units/systems/si/base.hpp>

namespace boost {

namespace units { 

namespace SI {

/// impedance : L^2 M T^-3 C^-2  
typedef derived_dimension<length_dim,2,mass_dim,1,time_dim,-3,current_dim,-2>::type   impedance_type;              

typedef unit<SI::impedance_type,SI::system>                                             impedance;
                                            
} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_IMPEDANCE_HPP
