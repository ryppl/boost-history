//-----------------------------------------------------------------------------
// boost mpl/test/logical.cpp source file
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

#include "boost/mpl/logical.hpp"
#include "boost/mpl/bool_t.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

struct incomplete;
    
int main()
{
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::true_t, mpl::true_t>::value == true));
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::false_t, mpl::true_t>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::true_t, mpl::false_t>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::false_t, mpl::false_t>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::false_t, incomplete>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_and<mpl::false_t, incomplete, incomplete>::value == false));

    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::true_t, mpl::true_t>::value == true));
    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::false_t, mpl::true_t>::value == true));
    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::true_t, mpl::false_t>::value == true));
    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::false_t, mpl::false_t>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::true_t, incomplete>::value == true));
    BOOST_STATIC_ASSERT((mpl::logical_or<mpl::true_t, incomplete, incomplete>::value == true));

    BOOST_STATIC_ASSERT((mpl::logical_not<mpl::true_t>::value == false));
    BOOST_STATIC_ASSERT((mpl::logical_not<mpl::false_t>::value == true));
  
    return 0;
}
