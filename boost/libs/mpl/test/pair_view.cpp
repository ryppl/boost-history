
// Copyright (c) David Abrahams 2003-2004
// Copyright (c) Aleksey Gurtovoy 2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/pair_view.hpp>
#include <boost/mpl/list/list10_c.hpp>
#include <boost/mpl/front.hpp>

#include <boost/mpl/aux_/test/test.hpp>


MPL_TEST_CASE( min_max_test )
{
    typedef list10_c<int,0,1,2,3,4,5,6,7,8,9> l1;
    typedef list10_c<int,9,8,7,6,5,4,3,2,1,0> l2;
    
    typedef pair_view<l1,l2> view;
    
    MPL_ASSERT_EQUAL(2,( front<view>::type::first::value,0 ));
    MPL_ASSERT_EQUAL(2,( front<view>::type::second::value,9 ));
}
