//-----------------------------------------------------------------------------
// boost mpl/arithmetic/negate.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_ARITHMETIC_NEGATE_HPP_INCLUDED
#define BOOST_MPL_ARITHMETIC_NEGATE_HPP_INCLUDED

#include "boost/mpl/integral_c.hpp"
#include "boost/mpl/aux_/typeof.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T)
    >
struct negate
{
    typedef BOOST_MPL_AUX_TYPEOF(T,T::value) value_type;
    BOOST_STATIC_CONSTANT(value_type, value = (-T::value));
#if !defined(__BORLANDC__)
    typedef integral_c<value_type, value> type;
#else
    typedef integral_c<value_type, (-T::value)> type;
#endif
};

BOOST_MPL_AUX_VOID_SPEC(1,negate)

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ARITHMETIC_NEGATE_HPP_INCLUDED
