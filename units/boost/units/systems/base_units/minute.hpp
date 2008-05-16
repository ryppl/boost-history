// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_MINUTE_BASE_UNIT_HPP
#define BOOST_UNITS_MINUTE_BASE_UNIT_HPP

#include <string>

#include <boost/units/config.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/systems/physical_dimensions/time.hpp>

namespace boost {

namespace units {

struct minute_base_unit : public base_unit<minute_base_unit, time_dimension, -13>
{
    static std::string name()   { return("minute"); }
    static std::string symbol() { return("min"); }
};

} // namespace units

} // namespace boost

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::units::minute_base_unit)

#endif

#include <boost/units/systems/base_units/detail/conversions.hpp>

#endif // BOOST_UNITS_MINUTE_BASE_UNIT_HPP
