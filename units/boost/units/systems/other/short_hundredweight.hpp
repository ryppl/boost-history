// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNIT_SYSTEMS_OTHER_SHORT_HUNDREDWEIGHT_HPP_INCLUDED
#define BOOST_UNIT_SYSTEMS_OTHER_SHORT_HUNDREDWEIGHT_HPP_INCLUDED

#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/units_fwd.hpp>
#include <boost/units/systems/other/avoirdupois_pound.hpp>

namespace boost {
namespace units {
namespace us {
typedef scaled_base_unit<avoirdupois_pound_base_unit, scale<10, static_rational<2> > > short_hundredweight_base_unit; //4.5359237e1*kilograms
}

template<>
struct base_unit_info<us::short_hundredweight_base_unit> {
    static const char* name() { return("short_hundredweight"); }
    static const char* symbol() { return("short_hundredweight"); }
};

}
}

#endif
