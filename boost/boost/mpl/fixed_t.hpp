//-----------------------------------------------------------------------------
// boost mpl/fixed_t.hpp header file
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

#ifndef BOOST_MPL_FIXED_T_HPP
#define BOOST_MPL_FIXED_T_HPP

#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<long IntegerPart, unsigned long FractionPart>
struct fixed_t
{
    BOOST_STATIC_CONSTANT(long, integer_part = IntegerPart);
    BOOST_STATIC_CONSTANT(unsigned long, fraction_part = FractionPart);
    typedef fixed_t<IntegerPart, FractionPart> type;
    
    // user-declared default ctor is required for definition of namespace 
    // scope constants
    fixed_t() {}
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FIXED_T_HPP
