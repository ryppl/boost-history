//-----------------------------------------------------------------------------
// boost mpl/test/bind.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/bind.hpp"
#include "boost/mpl/assert_is_same.hpp"

namespace mpl = boost::mpl;

struct test0
{
    typedef test0 type;
//    template<typename = void> struct apply
//    {
//       typedef test0 type;
//    };
};

struct test1
{
    template<typename A> struct apply
    {
        typedef A type;
    };
};

struct test2
{
    template<typename A1, typename A2> struct apply
    {
        typedef apply type;
    };
};

struct test3
{
    template<typename A1, typename A2, typename A3> struct apply
    {
        typedef apply type;
    };
};

struct test4
{
    template<typename A1, typename A2, typename A3, typename A4> struct apply
    {
        typedef apply type;
    };
};

struct test5
{
    template<typename A1, typename A2, typename A3, typename A4, typename A5> struct apply
    {
        typedef A5 type;
    };
};

int main()
{
    using namespace mpl::lambda_arg;

    typedef mpl::bind1<test1, _3>::type b1;
    typedef b1::apply<int, long, char>::type t1;

    typedef mpl::bind5<test5, char, int, short, long, _2>::type b5;
    typedef b5::apply<int, long>::type t2;

    BOOST_MPL_ASSERT_IS_SAME(t1, char);
    BOOST_MPL_ASSERT_IS_SAME(t2, long);


    typedef mpl::bind<test0>::type nested_bind_0;
    typedef mpl::bind<test1, _2>::type nested_bind_1;
    typedef mpl::bind<test2, _1, char>::type nested_bind_2;
    typedef mpl::bind<test3, void, _4, int>::type nested_bind_3;
    typedef mpl::bind<test4, nested_bind_0, _2, _1, _3>::type nested_bind_4;

    typedef mpl::bind<test4, nested_bind_1, nested_bind_2, nested_bind_3, nested_bind_4>::type bound;
    typedef bound::apply<void, char, long, float>::type result;
 
    return 0;
}
