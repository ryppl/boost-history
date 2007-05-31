// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_BASE_DIMENSIONS_HPP
#define BOOST_UNITS_BASE_DIMENSIONS_HPP

/// \file 
/// \brief SI base physical dimensions.
///
/// \detailed This file includes the seven SI-defined base dimensions as
/// well as derived dimensions for a number of commonly encountered
/// physical unit types. Angles and solid angles are technically dimensionless. 
/// However, incorrect use of degrees and radians is a common source of errors, 
/// so we treat them on the same footing as the seven physical dimensions.

#include <boost/units/systems/physical_dimensions/amount.hpp>
#include <boost/units/systems/physical_dimensions/current.hpp>
#include <boost/units/systems/physical_dimensions/length.hpp>
#include <boost/units/systems/physical_dimensions/luminous_intensity.hpp>
#include <boost/units/systems/physical_dimensions/mass.hpp>
#include <boost/units/systems/physical_dimensions/plane_angle.hpp>
#include <boost/units/systems/physical_dimensions/solid_angle.hpp>
#include <boost/units/systems/physical_dimensions/temperature.hpp>
#include <boost/units/systems/physical_dimensions/time.hpp>

#endif // BOOST_UNITS_BASE_DIMENSIONS_HPP
