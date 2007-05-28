// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MCS_TEST_SYSTEM_HPP
#define MCS_TEST_SYSTEM_HPP

#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/units/base_dimension.hpp>
#include <boost/units/derived_dimension.hpp>
#include <boost/units/io.hpp>
#include <boost/units/ordinal.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/make_system.hpp>

namespace boost {

namespace units {

//[test_system_snippet_1
struct length_dim : boost::units::base_dimension<length_dim, 1> {};                        ///> base dimension of length
struct mass_dim : boost::units::base_dimension<mass_dim,2> {};                             ///> base dimension of mass
struct time_dim : boost::units::base_dimension<time_dim,3> {};                             ///> base dimension of time
//]

#if 0
//[test_system_snippet_2
typedef make_dimension_list< boost::mpl::list< dim< length_dim,static_rational<1> > > >::type   length_type;
typedef make_dimension_list< boost::mpl::list< dim< mass_dim,static_rational<1> > > >::type     mass_type;
typedef make_dimension_list< boost::mpl::list< dim< time_dim,static_rational<1> > > >::type     time_type;
//]
#endif

//[test_system_snippet_3
typedef length_dim::type    length_type;
typedef mass_dim::type      mass_type;
typedef time_dim::type      time_type;
//]

#if 0
//[test_system_snippet_4
typedef make_dimension_list< boost::mpl::list< dim< length_dim,static_rational<2> > > >::type   area_type;
typedef make_dimension_list< boost::mpl::list< dim< mass_dim,static_rational<1> >,
                                               dim< length_dim,static_rational<2> >,
                                               dim< time_dim,static_rational<-2> > > >::type    energy_type;
//]
#endif

//[test_system_snippet_5
typedef derived_dimension<length_dim,2>::type area_type;
typedef derived_dimension<mass_dim,1,
                          length_dim,2,
                          time_dim,-2>::type  energy_type;
//]

namespace test {

//[test_system_snippet_6

struct length_unit : base_unit<length_unit, length_type, 1> {};
struct mass_unit : base_unit<mass_unit, mass_type, 2> {};
struct time_unit : base_unit<time_unit, time_type, 3> {};

typedef make_system<length_unit, mass_unit, time_unit>::type system;

/// unit typedefs
typedef unit<dimensionless_type,system>     dimensionless;

typedef unit<length_type,system>            length;
typedef unit<mass_type,system>              mass;
typedef unit<time_type,system>              time;

typedef unit<area_type,system>              area;
typedef unit<energy_type,system>            energy;
//]

//[test_system_snippet_7
/// unit constants 
BOOST_UNITS_STATIC_CONSTANT(meter,length);
BOOST_UNITS_STATIC_CONSTANT(meters,length);
BOOST_UNITS_STATIC_CONSTANT(kilogram,mass);
BOOST_UNITS_STATIC_CONSTANT(kilograms,mass);
BOOST_UNITS_STATIC_CONSTANT(second,time);
BOOST_UNITS_STATIC_CONSTANT(seconds,time);

BOOST_UNITS_STATIC_CONSTANT(square_meter,area);
BOOST_UNITS_STATIC_CONSTANT(square_meters,area);
BOOST_UNITS_STATIC_CONSTANT(joule,energy);
BOOST_UNITS_STATIC_CONSTANT(joules,energy);
//]

} // namespace test

//[test_system_snippet_8
template<> struct base_unit_info<test::length_unit>
{
    static std::string name()               { return "meter"; }
    static std::string symbol()             { return "m"; }
};
//]

template<> struct base_unit_info<test::mass_unit>
{
    static std::string name()               { return "kilogram"; }
    static std::string symbol()             { return "kg"; }
};

template<> struct base_unit_info<test::time_unit>
{
    static std::string name()               { return "second"; }
    static std::string symbol()             { return "s"; }
};

} // namespace units

} // namespace boost

#endif // MCS_TEST_SYSTEM_HPP
