//-----------------------------------------------------------------------------
// boost mpl/test/apply.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/apply.hpp"
#include "boost/mpl/assert_is_same.hpp"

namespace mpl = boost::mpl;

struct f1
{
    template<typename U> struct apply
    {
        typedef U type;
    };
};

struct f2
{
    template<typename U1, typename U2> struct apply
    {
        typedef U2 type;
    };
};

int main()
{
    typedef mpl::apply1<f1, int>::type t1;
    typedef mpl::apply2<f2, int, long>::type t2;

    BOOST_MPL_ASSERT_IS_SAME(t1, int);
    BOOST_MPL_ASSERT_IS_SAME(t2, long);

#if defined(BOOST_MPL_HAS_APPLY)
    typedef mpl::apply<f1, int>::type t3;
    typedef mpl::apply<f2, int, long>::type t4;
#endif


// should not compile
//    typedef mpl::apply0<f1, int>::type t0;
//    typedef mpl::apply1<f1, int, long>::type t1;
//    typedef mpl::apply2<f1, int>::type t2;
//    typedef mpl::apply2<f1, int, int>::type t3;

//    typedef mpl::apply1<f2, int>::type t4;
//    typedef mpl::apply2<f2, int>::type t5;

    return 0;
}
