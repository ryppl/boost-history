//-----------------------------------------------------------------------------
// boost mpl/bool_t.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_BOOL_T_HPP
#define BOOST_MPL_BOOL_T_HPP

#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<bool C>
struct bool_t
{
    BOOST_STATIC_CONSTANT(bool, value = C);
    typedef bool_t<C> type;
    
    operator bool() const { return this->value; } 
};

// "traditional" shorcuts for boolean type constants

typedef mpl::bool_t<true> true_t;
typedef mpl::bool_t<false> false_t;

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_BOOL_T_HPP
