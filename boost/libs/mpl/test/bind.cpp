
// Copyright (c) Peter Dimov 2001-2002
// Copyright (c) Aleksey Gurtovoy 2001-2004
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
#include <boost/mpl/aux_/test.hpp>

#include <boost/type_traits/is_same.hpp>

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
    typedef bind1<f1,_1>::apply<int>::type r11;
    typedef bind1<f1,_5>::apply<void,void,void,void,int>::type r12;
    MPL_ASSERT(( boost::is_same<r11,int> ));
    MPL_ASSERT(( boost::is_same<r12,int> ));
    
    typedef bind5<f5,_1,_2,_3,_4,_5>::apply<void,void,void,void,int>::type r51;
    typedef bind5<f5,_5,_4,_3,_2,_1>::apply<int,void,void,void,void>::type r52;
    MPL_ASSERT(( boost::is_same<r51,int> ));
    MPL_ASSERT(( boost::is_same<r52,int> ));
}


MPL_TEST_CASE() // fully bound metafunction classes
{
    typedef bind1<f1,int>::apply<>::type r11;
    typedef bind5<f5,void,void,void,void,int>::apply<>::type r51;
    MPL_ASSERT(( boost::is_same<r11,int> ));    
    MPL_ASSERT(( boost::is_same<r51,int> ));
}


MPL_TEST_CASE() // metafunction class composition
{
    typedef bind5<f5,_1,_2,_3,_4,bind1<f1,_1> >::apply<int,void,void,void,void>::type r51;
    typedef bind5<f5,_1,_2,_3,_4,bind1<f1,_5> >::apply<void,void,void,void,int>::type r52;
    MPL_ASSERT(( boost::is_same<r51,int> ));
    MPL_ASSERT(( boost::is_same<r52,int> ));
}
