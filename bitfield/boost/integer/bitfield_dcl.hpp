//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTEGER_BITFIELD_DCL__HPP
#define BOOST_INTEGER_BITFIELD_DCL__HPP

#include <boost/integer/bitfield.hpp>

#define BOOST_BITFIELD_DCL(STORAGE_TYPE, STORAGE_VAR, VALUE_TYPE, FIELD, F, L) \
   typedef boost::integer::bitfield_traits<STORAGE_TYPE,F,L,VALUE_TYPE>   FIELD##_type; \
   FIELD##_type::reference FIELD() { return FIELD##_type::reference(STORAGE_VAR); } \
   FIELD##_type::value::value_type FIELD() const { return FIELD##_type::value(word).get(); } \
   void set_##FIELD(VALUE_TYPE& val) { FIELD##_type::reference(STORAGE_VAR).set(val); } \
   FIELD##_type::value::value_type get_##FIELD() const { return FIELD##_type::value(word).get(); }

#endif
