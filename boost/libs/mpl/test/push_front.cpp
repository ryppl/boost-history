//-----------------------------------------------------------------------------
// boost mpl/test/push_front.cpp source file
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

#include "boost/mpl/push_front.hpp"
#include "boost/mpl/list.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::list<>::type types1;
    typedef mpl::list<long>::type types2;
    typedef mpl::list<int,long>::type types3;

    typedef mpl::push_front<types1,long>::type result1;
    typedef mpl::push_front<types2,int>::type  result2;
    typedef mpl::push_front<types3,char>::type result3;
    
    BOOST_STATIC_ASSERT(mpl::size<result1>::value == 1);
    BOOST_STATIC_ASSERT(mpl::size<result2>::value == 2);
    BOOST_STATIC_ASSERT(mpl::size<result3>::value == 3);
    
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<result1>::type, long);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<result2>::type, int);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<result3>::type, char);

    return 0;
}
