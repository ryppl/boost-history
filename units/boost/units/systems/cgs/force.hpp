// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_CGS_FORCE_HPP
#define BOOST_UNITS_CGS_FORCE_HPP

#include <boost/units/systems/cgs/base.hpp>

namespace boost {

namespace units { 

namespace CGS {

typedef unit<force_dim,CGS::system>        force;
    
BOOST_UNITS_STATIC_CONSTANT(dyne,force);
BOOST_UNITS_STATIC_CONSTANT(dynes,force);

} // namespace CGS

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_CGS_FORCE_HPP
