// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_MAGNETIC_FLUX_DENSITY_HPP
#define BOOST_UNITS_SI_MAGNETIC_FLUX_DENSITY_HPP

#include <iostream>
#include <boost/units/io.hpp>

#include <boost/units/derived_dimension.hpp>
#include <boost/units/systems/si/base.hpp>

namespace boost {

namespace units { 

namespace si {

/// magnetic flux density : M T^-2 I^-1
typedef derived_dimension<mass_base_dimension,1,time_base_dimension,-2,current_base_dimension,-1>::type    magnetic_flux_density_type;        

typedef unit<si::magnetic_flux_density_type,si::system>                     magnetic_flux_density;
    
BOOST_UNITS_STATIC_CONSTANT(tesla,magnetic_flux_density);   
BOOST_UNITS_STATIC_CONSTANT(teslas,magnetic_flux_density);  

} // namespace si

std::ostream& operator<<(std::ostream& os, const boost::units::si::magnetic_flux_density&) 
{
    return(os << "T");
}

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_MAGNETIC_FLUX_DENSITY_HPP
