// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNIT_SYSTEMS_OTHER_ARCSECOND_HPP_INCLUDED
#define BOOST_UNIT_SYSTEMS_OTHER_ARCSECOND_HPP_INCLUDED

//#include <boost/units/systems/detail/non_si_unit.hpp>
//#include <boost/units/systems/base_units/radian.hpp>

//BOOST_UNITS_NON_SI_UNIT(metric, arcsecond, arcsecond, (3.14159265358979323846264338328/648000), radian_base_unit, 9);

#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/scale.hpp>
#include <boost/units/systems/base_units/degree.hpp>

namespace boost {
namespace units {
namespace metric {

typedef scaled_base_unit<degree_base_unit, scale<60, static_rational<-2> > >  arcsecond_base_unit;

template<>
struct base_unit_info<metric::arcsecond_base_unit> {
    static const char* name()   { return("arcsecond"); }
    static const char* symbol() { return("\""); }
};

}
}
}

#endif
