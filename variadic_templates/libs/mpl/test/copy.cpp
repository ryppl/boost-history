
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/09/04 02:54:46 $
// $Revision: 1.7 $

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef vector_c<int,9,8,7,6,5,4,3,2,1,0> answer;
    typedef copy<
          range_c<int,0,10>
        , mpl::front_inserter< vector<> >
        >::type result;

    MPL_ASSERT_RELATION( size<result>::value, ==, 10 );
    MPL_ASSERT(( equal< result,answer > ));
}

MPL_TEST_CASE()
{
    typedef vector_c<int,10,11,12,13,14,15,16,17,18,19> numbers;
    typedef reverse_copy<
          range_c<int,0,10>
        , mpl::front_inserter<numbers>
        >::type result;

    MPL_ASSERT_RELATION( size<result>::value, ==,  20 );
    MPL_ASSERT(( equal< result,range_c<int,0,20> > ));
}

