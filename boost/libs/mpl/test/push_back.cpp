//-----------------------------------------------------------------------------
// boost mpl/test/push_back.cpp source file
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

#include "boost/mpl/push_back.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/back.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<> types1;
    typedef mpl::type_list<long> types2;
    typedef mpl::type_list<int, long> types3;

    typedef mpl::push_back<types1,long>::sequence types1_result;
    typedef mpl::push_back<types2,int>::sequence types2_result;
    typedef mpl::push_back<types3,char>::sequence types3_result;
    
    BOOST_STATIC_ASSERT(mpl::size<types1_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::size<types2_result>::value == 2);
    BOOST_STATIC_ASSERT(mpl::size<types3_result>::value == 3);
    
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<types1_result>::type, long);
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<types2_result>::type, int);
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<types3_result>::type, char);

    typedef mpl::value_list<> values1;
    typedef mpl::value_list<1> values2;
    typedef mpl::value_list<1,2> values3;

    typedef mpl::push_back<values1,mpl::int_t<1> >::sequence values1_result;
    typedef mpl::push_back<values2,mpl::int_t<2> >::sequence values2_result;
    typedef mpl::push_back<values3,mpl::int_t<3> >::sequence values3_result;

    BOOST_STATIC_ASSERT(mpl::size<values1_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::size<values2_result>::value == 2);
    BOOST_STATIC_ASSERT(mpl::size<values3_result>::value == 3);
    
    BOOST_STATIC_ASSERT(mpl::back<values1_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::back<values2_result>::value == 2);
    BOOST_STATIC_ASSERT(mpl::back<values3_result>::value == 3);

    return 0;
}
