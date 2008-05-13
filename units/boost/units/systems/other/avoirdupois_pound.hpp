// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNIT_SYSTEMS_OTHER_AVOIRDUPOIS_POUND_HPP_INCLUDED
#define BOOST_UNIT_SYSTEMS_OTHER_AVOIRDUPOIS_POUND_HPP_INCLUDED

#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/systems/other/avoirdupois_dram.hpp>

namespace boost {
namespace units {
namespace us {
typedef scaled_base_unit<avoirdupois_dram_base_unit, scale<2, static_rational<8> > > avoirdupois_pound_base_unit; //4.5359237e-1*kilograms
}
}
}

#endif
