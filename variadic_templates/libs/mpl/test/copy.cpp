
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/05/26 21:10:09 $
// $Revision: 1.5 $

#define USE_RANGE_C
#ifdef USE_RANGE_C
  #include <boost/mpl/range_c.hpp>
#else
  //disallow using range_c because currently 
  //(gcc-4.4-20090109) compiler buggy.
#endif  
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef vector_c<int,9,8,7,6,5,4,3,2,1,0> answer;
    typedef copy<
      #ifdef USE_RANGE_C
          range_c<int,0,10>
      #else
          vector_c<int,0,1,2,3,4,5,6,7,8,9>
      #endif
        , mpl::front_inserter< vector<> >
        >::type result;

    MPL_ASSERT_RELATION( size<result>::value, ==, 10 );
    MPL_ASSERT(( equal< result,answer > ));
}
#ifdef USE_RANGE_C
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
#endif
