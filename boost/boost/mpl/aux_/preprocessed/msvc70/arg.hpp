
// Copyright (c) Peter Dimov 2001-2002
// Copyright (c) Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/arg.hpp" header
// -- DO NOT modify by hand!

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
template<> struct arg< -1 >
{
    enum { value = -1 };
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U1 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<1>
{
    enum { value = 1 };
    typedef arg<2> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U1 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<2>
{
    enum { value = 2 };
    typedef arg<3> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U2 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<3>
{
    enum { value = 3 };
    typedef arg<4> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U3 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<4>
{
    enum { value = 4 };
    typedef arg<5> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U4 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<5>
{
    enum { value = 5 };
    typedef arg<6> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U5 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

BOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1,int, arg)

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
