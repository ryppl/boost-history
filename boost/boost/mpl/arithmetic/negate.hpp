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
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template< typename T >
struct negate
{
    typedef typename BOOST_MPL_AUX_TYPEOF(T::value) value_t;
    BOOST_STATIC_CONSTANT(value_t, value = (-T::value));
    typedef integral_c<value_t, value> type;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ARITHMETIC_NEGATE_HPP_INCLUDED
