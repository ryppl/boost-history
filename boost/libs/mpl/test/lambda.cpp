//-----------------------------------------------------------------------------
// boost mpl/test/lambda.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/logical.hpp"
#include "boost/mpl/comparison.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/mpl/int_c.hpp"
#include "boost/mpl/bool_c.hpp"
#include "boost/mpl/utility/size_of.hpp"

#include "boost/type_traits/same_traits.hpp"
#include "boost/type_traits/arithmetic_traits.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

// need to wrap "legacy" metafunctions 
template<typename T1, typename T2> struct is_same
    : mpl::pred< boost::is_same<T1, T2> > {};

template<typename T> struct is_float
    : mpl::pred< boost::is_float<T> > {};

struct my
{
    char a[100];
};

int main()
{
    // !(x == char) && !(x == double) && x convertible to int || size_of(x) > 8
    typedef mpl::lambda<
        mpl::logical_or<
              mpl::logical_and<
                    mpl::logical_not< is_same<mpl::_1,char> >
                  , mpl::logical_not< is_float<mpl::_1> >
                  >
            , mpl::greater< mpl::size_of<mpl::_1>, mpl::int_c<8> >
            >
        >::type f1;

    BOOST_STATIC_ASSERT(!f1::apply<char>::type::value);
    BOOST_STATIC_ASSERT(!f1::apply<double>::type::value);
    BOOST_STATIC_ASSERT(f1::apply<long>::type::value);
    BOOST_STATIC_ASSERT(f1::apply<my>::type::value);

    // x == y || x == my || sizeof(x) == sizeof(y)
    typedef mpl::lambda<
        mpl::logical_or< 
              is_same<mpl::_1,mpl::_2>
            , is_same<mpl::_2,my>
            , mpl::equal_to< mpl::size_of<mpl::_1>, mpl::size_of<mpl::_2> >
            >
        >::type f2;

    BOOST_STATIC_ASSERT((!f2::apply<double,char>::type::value));
    BOOST_STATIC_ASSERT((!f2::apply<my,int>::type::value));
    BOOST_STATIC_ASSERT((!f2::apply<my,char[99]>::type::value));
    BOOST_STATIC_ASSERT((f2::apply<int,int>::type::value));
    BOOST_STATIC_ASSERT((f2::apply<void,my>::type::value));
    BOOST_STATIC_ASSERT((f2::apply<signed long, unsigned long>::type::value));

    return 0;
}
