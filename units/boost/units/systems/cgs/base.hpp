// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_CGS_BASE_HPP
#define BOOST_UNITS_CGS_BASE_HPP

#include <string>

#include <boost/units/io.hpp>
#include <boost/units/ordinal.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/unit.hpp>
#include <boost/units/system.hpp>
#include <boost/units/systems/physical_units.hpp>

#include <boost/units/experimental/make_system.hpp>
#include <boost/units/experimental/fundamental_units.hpp>

namespace boost {

namespace units { 

namespace CGS {

/// placeholder class defining CGS unit system
//struct system_tag : public ordinal<-4> { };
typedef make_system<centimeter_tag, gram_tag, second_tag>::type system;

}

}

}

//#if BOOST_UNITS_HAS_BOOST_TYPEOF
//
//#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
//
//BOOST_TYPEOF_REGISTER_TYPE(boost::units::CGS::system_tag)
//
//#endif

namespace boost {

namespace units {

namespace CGS {
    
//typedef homogeneous_system<system_tag>  system;

/// various unit typedefs for convenience
typedef unit<dimensionless_type,system>         dimensionless;

} // namespace CGS
                                                    
//template<> 
//struct base_unit_info<length_dim,CGS::system_tag> 
//{ 
//    static std::string name()       { return "centimeter"; }
//    static std::string symbol()     { return "cm"; }
//};
//    
//template<> 
//struct base_unit_info<mass_dim,CGS::system_tag> 
//{ 
//    static std::string name()       { return "gram"; }
//    static std::string symbol()     { return "g"; }
//};
//    
//template<> 
//struct base_unit_info<time_dim,CGS::system_tag> 
//{ 
//    static std::string name()       { return "second"; }
//    static std::string symbol()     { return "s"; }
//};

} // namespace units

} // namespace boost

//#include <boost/units/systems/conversions/conversion_headers.hpp>

#endif // BOOST_UNITS_CGS_BASE_HPP
