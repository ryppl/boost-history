//-----------------------------------------------------------------------------
// boost mpl/test/int_t.cpp source file
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

#include "boost/mpl/int_t.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    BOOST_STATIC_ASSERT(mpl::int_t<0>::value == 0);
    BOOST_STATIC_ASSERT(mpl::int_t<1>::value == 1);
    BOOST_STATIC_ASSERT(mpl::int_t<-1>::value == -1);
    BOOST_STATIC_ASSERT(mpl::int_t<10>::value == 10);
    BOOST_STATIC_ASSERT(mpl::int_t<-10>::value == -10);
    
    BOOST_MPL_ASSERT_IS_SAME(mpl::int_t<0>::type, mpl::int_t<0>);
    BOOST_MPL_ASSERT_IS_SAME(mpl::int_t<1>::type, mpl::int_t<1>);
    BOOST_MPL_ASSERT_IS_SAME(mpl::int_t<-1>::type, mpl::int_t<-1>);
    BOOST_MPL_ASSERT_IS_SAME(mpl::int_t<10>::type, mpl::int_t<10>);
    BOOST_MPL_ASSERT_IS_SAME(mpl::int_t<-10>::type, mpl::int_t<-10>);

    return 0;
}
