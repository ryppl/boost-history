// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_STERADIAN_BASE_UNIT_HPP
#define BOOST_UNITS_STERADIAN_BASE_UNIT_HPP

#include <string>

#include <boost/units/config.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/systems/base_dimensions/solid_angle_base_dimension.hpp>

namespace boost {

namespace units {

struct steradian_base_unit : public base_unit<steradian_base_unit, solid_angle_dimension, -1>
{
    static std::string name() { return("steradian"); }
    static std::string symbol() { return("sr"); }
};

} // namespace units

} // namespace boost

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::units::steradian_base_unit)

#endif

#endif // BOOST_UNITS_STERADIAN_BASE_UNIT_HPP
