//-----------------------------------------------------------------------------
// boost mpl/same_as.hpp header file
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

#ifndef BOOST_MPL_SAME_AS_HPP
#define BOOST_MPL_SAME_AS_HPP

#include "boost/mpl/bool_t.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename T1>
struct same_as
{
    template<typename T2> struct apply
    {
        BOOST_STATIC_CONSTANT(bool, value =
            (::boost::is_same<T1, T2>::value)
            );
        
        typedef mpl::bool_t<value> type;
    };
};

template<typename T1>
struct not_same_as
{
    template<typename T2> struct apply
    {
        BOOST_STATIC_CONSTANT(bool, value =
            (!::boost::is_same<T1, T2>::value)
            );
        
        typedef mpl::bool_t<value> type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_SAME_AS_HPP
