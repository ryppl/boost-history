// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_RESISTANCE_HPP
#define BOOST_UNITS_SI_RESISTANCE_HPP

#include <boost/units/systems/si/base.hpp>

namespace boost {

namespace units { 

namespace SI {

/// resistance : L^2 M T^-3 C^-2  
typedef composite_dimension<length_tag,2,mass_tag,1,time_tag,-3,current_tag,-2>::type   resistance_type;              

typedef unit<SI::resistance_type,SI::system>                                            resistance;

BOOST_UNITS_STATIC_CONSTANT(ohm,resistance);    
BOOST_UNITS_STATIC_CONSTANT(ohms,resistance);   
                                            
} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_RESISTANCE_HPP
