
#ifndef BOOST_MPL_VOID_HPP_INCLUDED
#define BOOST_MPL_VOID_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/adl_barrier.hpp>

// should be the last include
#include <boost/type_traits/detail/bool_trait_def.hpp>

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

//  [JDG Feb-4-2003] made void_ a complete type to allow it to be
//  instantiated so that it can be passed in as an object that can be
//  used to select an overloaded function. Possible use includes signaling
//  a zero arity functor evaluation call.
struct void_ { typedef void_ type; };

BOOST_TT_AUX_BOOL_TRAIT_DEF1( is_void_, T, false )
BOOST_TT_AUX_BOOL_TRAIT_SPEC1( is_void_, void_, true )

BOOST_TT_AUX_BOOL_TRAIT_DEF1( is_not_void_, T, true )
BOOST_TT_AUX_BOOL_TRAIT_SPEC1( is_not_void_, void_, false )

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
BOOST_MPL_AUX_ADL_BARRIER_DECL(void_)

#include <boost/type_traits/detail/bool_trait_undef.hpp>

#endif // BOOST_MPL_VOID_HPP_INCLUDED
