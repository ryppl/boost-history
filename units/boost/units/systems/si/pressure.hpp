// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_PRESSURE_HPP
#define BOOST_UNITS_SI_PRESSURE_HPP

#include <boost/units/systems/si/base.hpp>
#include <boost/units/systems/derived_dimensions/pressure_derived_dimension.hpp>

namespace boost {

namespace units { 

namespace SI {

typedef unit<pressure_dimension,SI::system>      pressure;

// windef.h #defines pascal on Metrowerks compilers
#if defined(__MWERKS__)
   #if !__option(only_std_keywords)
       #define BOOST_UNITS_NO_PASCAL 1
   #elif defined(pascal)
       #define BOOST_UNITS_NO_PASCAL 1
   #endif
#elif defined(pascal)
   #define BOOST_UNITS_NO_PASCAL 1
#elif BOOST_MSVC
   #define BOOST_UNITS_NO_PASCAL 1
#endif

#ifndef BOOST_UNITS_NO_PASCAL
BOOST_UNITS_STATIC_CONSTANT(pascal,pressure);   
#endif
BOOST_UNITS_STATIC_CONSTANT(pascals,pressure);  

} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_PRESSURE_HPP
