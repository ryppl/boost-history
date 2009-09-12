
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/09/10 22:53:37 $
// $Revision: 1.1 $

#include <boost/mpl/list_c.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/static_assert.hpp>

#include <boost/mpl/aux_/test.hpp>


//!@nv-mpl_diff:
//!  WHAT:
//!    1) expresstions such as:
//!         list_c<T,c1,c2,...>::type
//!       were replaced with just:
//!         list_c<T,c1,c2,...>
//!  WHY:
//!    1) list_c<T,c1,c2,...> was changed
//!       to inheirit from:
//!       l_item<integral_c<T,c1>,Next>
//!       where Next was a function of <T,c2,...>
//!       , and the only purpose of purpose
//!       of using list_c<T,c1,c2,...>::type
//!       instead of l_item<integral-c<C1,c1>,Next>
//!       was to provide another name for T.
//!       As defined here, T could simply be
//!       gotten by front<list_c<T,c1...> >::value_type;
//!       hence, there's no real need for
//!       list_c<C1,c1,c2,...>::type.
//!    2) Trying to emulate more precisely the non-variadic
//!       list_c was proving too difficult.
//!    3) The only mention of list_c<T,c1,c2,...>::type
//!       (actually seq_c<T,c1,c2,...>) in:
//!         http://www.boost.org/doc/libs/1_40_0/libs/mpl/doc/refmanual/integral-sequence-wrapper.html
//!       is with seqn_c<T1,c1,c2,... cn>.  Since there's no 
//!       numbered sequences in variadic mpl, that specification
//!       doesn't apply.

#if !BOOST_WORKAROUND(BOOST_MSVC,<= 1200)
MPL_TEST_CASE()
{
    typedef list_c<bool,true> l1;
    typedef list_c<bool,false> l2;

    MPL_ASSERT(( is_same< l1::value_type, bool > ));
    MPL_ASSERT(( is_same< l2::value_type, bool > ));

    MPL_ASSERT_RELATION( front<l1>::type::value, ==, true );
    MPL_ASSERT_RELATION( front<l2>::type::value, ==, false );
}
#endif

MPL_TEST_CASE()
{
    typedef list_c<int,-1> l1;
    typedef list_c<int,0,1> l2;
    typedef list_c<int,1,2,3> l3;

    MPL_ASSERT(( is_same< l1::value_type, int > ));
    MPL_ASSERT(( is_same< l2::value_type, int > ));
    MPL_ASSERT(( is_same< l3::value_type, int > ));

    MPL_ASSERT_RELATION( size<l1>::value, ==, 1 );
    MPL_ASSERT_RELATION( size<l2>::value, ==, 2 );
    MPL_ASSERT_RELATION( size<l3>::value, ==, 3 );
    MPL_ASSERT_RELATION( front<l1>::type::value, ==, -1 );
    MPL_ASSERT_RELATION( front<l2>::type::value, ==, 0 );
    MPL_ASSERT_RELATION( front<l3>::type::value, ==, 1 );
}

MPL_TEST_CASE()
{
    typedef list_c<unsigned,0> l1;
    typedef list_c<unsigned,1,2> l2;

    MPL_ASSERT(( is_same< l1::value_type, unsigned > ));
    MPL_ASSERT(( is_same< l2::value_type, unsigned > ));

    MPL_ASSERT_RELATION( size<l1>::value, ==, 1 );
    MPL_ASSERT_RELATION( size<l2>::value, ==, 2 );
    MPL_ASSERT_RELATION( front<l1>::type::value, ==, 0 );
    MPL_ASSERT_RELATION( front<l2>::type::value, ==, 1 );
}

MPL_TEST_CASE()
{
    typedef list_c<unsigned,2,1> l2;

    MPL_ASSERT(( is_same< l2::value_type, unsigned > ));
    
    typedef begin<l2>::type i1;
    typedef next<i1>::type  i2;
    typedef next<i2>::type  i3;
    
    MPL_ASSERT_RELATION( deref<i1>::type::value, ==, 2 );
    MPL_ASSERT_RELATION( deref<i2>::type::value, ==, 1 );
    MPL_ASSERT(( is_same< i3, end<l2>::type > ));
}
