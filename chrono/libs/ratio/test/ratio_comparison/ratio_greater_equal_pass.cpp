//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//  Adaptation to Boost of the libcxx
//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/ratio.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

#define BOOST_RATIO_INTMAX_T_MAX (0x7FFFFFFFFFFFFFFFLL-1)

void test()
{
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
//    {
//    typedef std::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R1;
//    typedef std::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R2;
//    BOOST_RATIO_STATIC_ASSERT((std::ratio_greater_equal<R1, R2>::value), NOTHING, ());
//    }
//    {
//    typedef std::ratio<1,BOOST_RATIO_INTMAX_T_MAX> R1;
//    typedef std::ratio<1,BOOST_RATIO_INTMAX_T_MAX> R2;
//    BOOST_RATIO_STATIC_ASSERT((std::ratio_greater_equal<R1, R2>::value), NOTHING, ());
//    }
    {
        typedef boost::mpl::integral_c<long long int, 0xffffffffffffffffLL> LL;
        typedef boost::mpl::integral_c<boost::intmax_t, LLONG_MAX> LL2;
        typedef boost::mpl::integral_c<boost::intmax_t, -LLONG_MAX> LL3;
        boost::intmax_t i = boost::integer::static_signed_sign<BOOST_RATIO_INTMAX_T_MAX>::value;
        boost::intmax_t j = boost::integer::static_signed_abs<BOOST_RATIO_INTMAX_T_MAX-1>::value;
        typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R1;
        typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R2;
//    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
#if 0
    {
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R1;
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R1;
    typedef boost::ratio<1, -BOOST_RATIO_INTMAX_T_MAX> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
#endif
}
