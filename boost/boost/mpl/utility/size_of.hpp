//-----------------------------------------------------------------------------
// boost mpl/utility/size_of.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_UTILITY_SIZE_OF_HPP_INCLUDED
#define BOOST_MPL_UTILITY_SIZE_OF_HPP_INCLUDED

#include "boost/mpl/integral_c.hpp"
#include "boost/config.hpp"

#include <cstddef> // for std::size_t

namespace boost {
namespace mpl {

template<typename T>
struct size_of
{
    BOOST_STATIC_CONSTANT(std::size_t, value = sizeof(T));
    typedef mpl::integral_c<std::size_t,value> type;    
};

} // namespace mpl
} // namespace boost 

#endif // #ifndef BOOST_MPL_UTILITY_SIZE_OF_HPP_INCLUDED
