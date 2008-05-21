// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_CONDUCTANCE_HPP
#define BOOST_UNITS_SI_CONDUCTANCE_HPP

#include <ostream>
#include <boost/units/io.hpp>

#include <boost/units/systems/si/base.hpp>
#include <boost/units/physical_dimensions/conductance.hpp>

namespace boost {

namespace units { 

namespace si {

typedef unit<conductance_type,si::system>    conductance;

BOOST_UNITS_STATIC_CONSTANT(siemen,conductance);    
BOOST_UNITS_STATIC_CONSTANT(siemens,conductance);   
BOOST_UNITS_STATIC_CONSTANT(mho,conductance);   
BOOST_UNITS_STATIC_CONSTANT(mhos,conductance);  

} // namespace si

inline std::ostream& operator<<(std::ostream& os, const boost::units::si::conductance&) 
{
    return(os << "S");
}

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_CONDUCTANCE_HPP
