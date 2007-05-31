// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_CGS_WAVENUMBER_HPP
#define BOOST_UNITS_CGS_WAVENUMBER_HPP

#include <boost/units/systems/cgs/base.hpp>
#include <boost/units/systems/derived_dimensions/wavenumber_derived_dimension.hpp>

namespace boost {

namespace units { 

namespace CGS {

typedef unit<wavenumber_dimension,CGS::system>   wavenumber;
    
BOOST_UNITS_STATIC_CONSTANT(kayser,wavenumber); 
BOOST_UNITS_STATIC_CONSTANT(kaysers,wavenumber);    
BOOST_UNITS_STATIC_CONSTANT(reciprocal_centimeter,wavenumber);  
BOOST_UNITS_STATIC_CONSTANT(reciprocal_centimeters,wavenumber); 
BOOST_UNITS_STATIC_CONSTANT(reciprocal_centimetre,wavenumber);  
BOOST_UNITS_STATIC_CONSTANT(reciprocal_centimetres,wavenumber); 

} // namespace CGS

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_CGS_WAVENUMBER_HPP
