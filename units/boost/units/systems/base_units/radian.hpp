// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_RADIAN_BASE_UNIT_HPP
#define BOOST_UNITS_RADIAN_BASE_UNIT_HPP

#include <string>

#include <boost/units/config.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/systems/physical_dimensions/plane_angle.hpp>

namespace boost {

namespace units {

struct radian_base_unit : public base_unit<radian_base_unit, plane_angle_dimension, -4>
{
    static std::string name()   { return("radian"); }
    static std::string symbol() { return("rad"); }
};

} // namespace units

} // namespace boost

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::units::radian_base_unit)


#endif

#endif // BOOST_UNITS_RADIAN_BASE_UNIT_HPP
