
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/03/28 15:38:35 $
// $Revision: 1.2 $

#include <boost/mpl/push_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef push_front<list<>,long>::type res1;
    typedef push_front<list<long>,int>::type res2;
    typedef push_front<list<int,long>,char>::type res3;
    
    MPL_ASSERT_RELATION( size<res1>::value, ==, 1 );
    MPL_ASSERT_RELATION( size<res2>::value, ==, 2 );
    MPL_ASSERT_RELATION( size<res3>::value, ==, 3 );
    
    MPL_ASSERT(( is_same< front<res1>::type, long > ));
    MPL_ASSERT(( is_same< front<res2>::type, int > ));
    MPL_ASSERT(( is_same< front<res3>::type, char > ));
    
    MPL_ASSERT(( has_push_front< list<> > ));
    MPL_ASSERT(( has_push_front< list<long> > ));

    MPL_ASSERT_NOT(( has_push_back< list<> > ));
}
