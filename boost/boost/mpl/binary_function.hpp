//-----------------------------------------------------------------------------
// boost mpl/binary_function.hpp header file
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

#ifndef BOOST_MPL_BINARY_FUNCTION_HPP
#define BOOST_MPL_BINARY_FUNCTION_HPP

#include "boost/mpl/mpl_config.hpp"

namespace boost {
namespace mpl {

template<typename BinaryFunction, typename T1, typename T2>
struct binary_function_body
{
    BOOST_MPL_DEPENDENT_TEMPLATE_TYPEDEF2(BinaryFunction, apply, T1, T2, type);
};

template<typename BinaryFunction, typename T1, typename T2>
struct binary_function
    : binary_function_body<BinaryFunction,T1,T2>::type
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_BINARY_FUNCTION_HPP
