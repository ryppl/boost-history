//-----------------------------------------------------------------------------
// boost mpl/test/pop_front.cpp source file
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

#include "boost/mpl/pop_front.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<long> types1;
    typedef mpl::type_list<int,long> types2;
    typedef mpl::type_list<char,int,long> types3;

    typedef mpl::pop_front<types1>::sequence types1_result;
    typedef mpl::pop_front<types2>::sequence types2_result;
    typedef mpl::pop_front<types3>::sequence types3_result;
    
    BOOST_STATIC_ASSERT(mpl::size<types1_result>::value == 0);
    BOOST_STATIC_ASSERT(mpl::size<types2_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::size<types3_result>::value == 2);
    
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<types2_result>::type, long);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<types3_result>::type, int);

    typedef mpl::value_list<1> values1;
    typedef mpl::value_list<2,1> values2;
    typedef mpl::value_list<3,2,1> values3;

    typedef mpl::pop_front<values1>::sequence values1_result;
    typedef mpl::pop_front<values2>::sequence values2_result;
    typedef mpl::pop_front<values3>::sequence values3_result;

    BOOST_STATIC_ASSERT(mpl::size<values1_result>::value == 0);
    BOOST_STATIC_ASSERT(mpl::size<values2_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::size<values3_result>::value == 2);
    
    BOOST_STATIC_ASSERT(mpl::front<values2_result>::value == 1);
    BOOST_STATIC_ASSERT(mpl::front<values3_result>::value == 2);

    return 0;
}
