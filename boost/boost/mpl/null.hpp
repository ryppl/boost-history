//-----------------------------------------------------------------------------
// boost mpl/null.hpp header file
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

#ifndef BOOST_MPL_NULL_HPP
#define BOOST_MPL_NULL_HPP

#include "boost/mpl/int_t.hpp"
#include <climits>

namespace boost {
namespace mpl {

struct null_argument;
typedef mpl::int_t<LONG_MAX> null_t;

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_NULL_HPP
