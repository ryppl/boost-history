//-----------------------------------------------------------------------------
// boost mpl/type_compare.hpp header file
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

#ifndef BOOST_MPL_TYPE_COMPARE_HPP
#define BOOST_MPL_TYPE_COMPARE_HPP

#include "boost/config.hpp"
#include "boost/type_traits/same_traits.hpp"

namespace boost {
namespace mpl {

template<
      typename Iterator1
    , typename Iterator2
    >
struct type_compare
{
    BOOST_STATIC_CONSTANT(bool, value = 
        (::boost::is_same<
                typename Iterator1::type
              , typename Iterator2::type
              >::value)
        );
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_TYPE_COMPARE_HPP
