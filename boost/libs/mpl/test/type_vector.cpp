//-----------------------------------------------------------------------------
// boost mpl/test/type_vector.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/type_vector.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/insert.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/back.hpp"
#include "boost/mpl/assert_is_same.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_vector<char, short, int, long, double> types;
    typedef mpl::begin<types>::iterator first;
    typedef mpl::end<types>::iterator last;

    BOOST_MPL_ASSERT_IS_SAME(mpl::back<types>::type, double);
    BOOST_MPL_ASSERT_IS_SAME(first::type, char);
    BOOST_MPL_ASSERT_IS_SAME(first::advance<5>::iterator, last);
    BOOST_MPL_ASSERT_IS_SAME(last::advance<-5>::iterator, first);
    BOOST_MPL_ASSERT_IS_SAME(first::advance<2>::iterator::type, int);
    
    typedef mpl::insert<types, first::next, void>::sequence t1;
    typedef mpl::push_back<t1, float>::sequence t2;
    typedef mpl::push_front<t2, unsigned>::sequence t3;

    BOOST_MPL_ASSERT_IS_SAME(mpl::front<t1>::type, char);
    BOOST_MPL_ASSERT_IS_SAME(mpl::begin<t1>::iterator::next::type, void);
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<t1>::type, double);
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<t2>::type, float);
    BOOST_MPL_ASSERT_IS_SAME(mpl::front<t3>::type, unsigned);
    BOOST_MPL_ASSERT_IS_SAME(mpl::back<t3>::type, float);
    
}
