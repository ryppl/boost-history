
// Copyright Peter Dimov 2001-2002
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/bind.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/aux_/test.hpp>

namespace {

struct f1
{
    template< typename T1 > struct apply
    {
        typedef T1 type;
    };
};

struct f5
{
    template< typename T1, typename T2, typename T3, typename T4, typename T5 >
    struct apply
    {
        typedef T5 type;
    };
};

} // namespace

MPL_TEST_CASE() // basic argument binding
{
    typedef apply_wrap1< bind1<f1,_1>, int >::type r11;
    typedef apply_wrap5< bind1<f1,_5>, void,void,void,void,int >::type r12;
    MPL_ASSERT(( boost::is_same<r11,int> ));
    MPL_ASSERT(( boost::is_same<r12,int> ));
    
    typedef apply_wrap5< bind5<f5,_1,_2,_3,_4,_5>, void,void,void,void,int >::type r51;
    typedef apply_wrap5< bind5<f5,_5,_4,_3,_2,_1>, int,void,void,void,void >::type r52;
    MPL_ASSERT(( boost::is_same<r51,int> ));
    MPL_ASSERT(( boost::is_same<r52,int> ));
}


MPL_TEST_CASE() // fully bound metafunction classes
{
    typedef apply_wrap0< bind1<f1,int> >::type r11;
    typedef apply_wrap0< bind5<f5,void,void,void,void,int> >::type r51;
    MPL_ASSERT(( boost::is_same<r11,int> ));
    MPL_ASSERT(( boost::is_same<r51,int> ));
}


MPL_TEST_CASE() // metafunction class composition
{
    typedef apply_wrap5< bind5<f5,_1,_2,_3,_4,bind1<f1,_1> >, int,void,void,void,void >::type r51;
    typedef apply_wrap5< bind5<f5,_1,_2,_3,_4,bind1<f1,_5> >, void,void,void,void,int >::type r52;
    MPL_ASSERT(( boost::is_same<r51,int> ));
    MPL_ASSERT(( boost::is_same<r52,int> ));
}
