// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file
    
\brief test_absolute.cpp

\detailed
Test absolute units.

Output:
@verbatim
@endverbatim
**/

#include <boost/units/quantity.hpp>
#include <boost/units/absolute.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/systems/derived_dimensions/temperature_derived_dimension.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/systems/base_units.hpp>

#include <iostream>

#include <boost/test/minimal.hpp>

#define BOOST_UNITS_CHECK_CLOSE(a, b) (BOOST_CHECK((std::abs((a) - (b)) < .0000001)))

namespace bu = boost::units;

typedef bu::unit<bu::temperature_dimension,bu::make_system<bu::kelvin_tag>::type> kelvin_type;

typedef bu::unit<bu::temperature_dimension,bu::make_system<bu::celsius_tag>::type> celsius_type;

typedef bu::unit<bu::temperature_dimension,bu::make_system<bu::fahrenheit_tag>::type> fahrenheit_type;

int test_main(int,char *[])
{
    bu::quantity<bu::absolute<fahrenheit_type> > q1(212.0 * bu::absolute<fahrenheit_type>());
    bu::quantity<bu::absolute<celsius_type> > q2(0.0 * bu::absolute<celsius_type>());
    bu::quantity<bu::absolute<fahrenheit_type> > q3(q2);
    bu::quantity<fahrenheit_type> q4(q1 - q3);

    BOOST_UNITS_CHECK_CLOSE(q4.value(), 180.0);

    bu::quantity<bu::absolute<kelvin_type> > q5(static_cast<bu::quantity<kelvin_type> >(q4) + static_cast<bu::quantity<bu::absolute<kelvin_type> > >(q2));

    BOOST_UNITS_CHECK_CLOSE(q5.value(), 373.15);

    bu::quantity<bu::absolute<fahrenheit_type> > q6(q5);

    BOOST_UNITS_CHECK_CLOSE(q6.value(), 212.0);

    return(0);
}
