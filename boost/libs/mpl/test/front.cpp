//-----------------------------------------------------------------------------
// boost mpl/test/front.cpp source file
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

#include "boost/mpl/front.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<long> types1;
    typedef mpl::type_list<int,long> types2;
    typedef mpl::type_list<char,int,long> types3;

    BOOST_MPL_ASSERT_IS_SAME(mpl::front<types1>::type, long);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<types2>::type, int);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<types3>::type, char);

    typedef mpl::value_list<1> values1;
    typedef mpl::value_list<2,1> values2;
    typedef mpl::value_list<3,2,1> values3;
    
    BOOST_STATIC_ASSERT(mpl::front<values1>::value == 1);
    BOOST_STATIC_ASSERT(mpl::front<values2>::value == 2);
    BOOST_STATIC_ASSERT(mpl::front<values3>::value == 3);

    return 0;
}
