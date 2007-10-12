// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_UNITSFWD_HPP
#define BOOST_UNITS_UNITSFWD_HPP

/// \file
/// Forward declarations of library components.

#ifndef BOOST_UNITS_DOXYGEN

namespace boost {

namespace units {

template<typename T,typename V> struct dim;
template<typename T> struct is_dim;

struct dimensionless_type;
template<class Item,class Next> struct dimension_list;
template<typename Seq> struct make_dimension_list;

template<class T> struct is_dimensionless;
template<class S1,class S2> struct is_implicitly_convertible;
template<class T> struct get_dimension;
template<class T> struct get_system;

template<class Dim,class System, class Enable=void> class unit;
template<class Y> class absolute;

template<class BaseUnitTag> struct base_unit_info;
template<class System> struct dimensionless_unit;
template<class T> struct is_unit;
template<class T,class Dim> struct is_unit_of_dimension;
template<class T,class System> struct is_unit_of_system;

template<class Unit,class Y> class quantity;

template<class System,class Y> struct dimensionless_quantity;
template<class T> struct is_quantity;
template<class T,class Dim> struct is_quantity_of_dimension;
template<class T,class System> struct is_quantity_of_system;

} // namespace units

} // namespace boost

#endif

#endif // BOOST_UNITS_UNITSFWD_HPP
