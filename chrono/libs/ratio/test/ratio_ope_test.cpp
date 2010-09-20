//  ratio_test.cpp  ----------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// A lot of test are taken from llvm/libcxx/test/utilities/ratio
//===---------------------------- ratio -----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <boost/ratio.hpp>
#include <iostream>

#if !defined(BOOST_NO_STATIC_ASSERT) || !defined(BOOST_RATIO_USES_MPL_ASSERT)
#define NOTHING ""
#endif


#ifndef BOOST_NO_STATIC_ASSERT
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(BOOST_RATIO_USES_STATIC_ASSERT)
#include <boost/static_assert.hpp>
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) BOOST_STATIC_ASSERT(CND)
#elif defined(BOOST_RATIO_USES_MPL_ASSERT)
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    BOOST_MPL_ASSERT_MSG(boost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#else
//~ #elif defined(BOOST_RATIO_USES_ARRAY_ASSERT)
#define BOOST_RATIO_CONCAT(A,B) A##B
#define BOOST_RATIO_NAME(A,B) BOOST_RATIO_CONCAT(A,B)
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static char BOOST_RATIO_NAME(__boost_ratio_test_,__LINE__)[(CND)?1:-1]
//~ #define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)
#endif


typedef boost::ratio<5, 3>   five_thirds;       // five_thirds::num == 5, five_thirds::den == 3
typedef boost::ratio<25, 15> also_five_thirds;  // also_five_thirds::num == 5, also_five_thirds::den == 3
typedef boost::ratio_divide<five_thirds, also_five_thirds>::type one;  // one::num == 1, one::den == 1


typedef boost::ratio_multiply<boost::ratio<5>, boost::giga>::type _5giga;  // _5giga::num == 5000000000, _5giga::den == 1
typedef boost::ratio_multiply<boost::ratio<5>, boost::nano>::type _5nano;  // _5nano::num == 1, _5nano::den == 200000000

//  Test the case described in library working group issue 948.

typedef boost::ratio<BOOST_INTMAX_C(0x7FFFFFFFFFFFFFFF), BOOST_INTMAX_C(0x7FFFFFFFFFFFFFF0)> R1;
typedef boost::ratio<8, 7> R2;
typedef boost::ratio_multiply<R1, R2>::type RT;



int main()
{

    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 2 && R::den == 1, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 3 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<-1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, -2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<-1, 1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, -1> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<56987354, 467584654> R1;
    typedef boost::ratio<544668, 22145> R2;
    typedef boost::ratio_add<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 127970191639601LL && R::den == 5177331081415LL, NOTHING, ());
    }
    
    {
    typedef boost::ratio<8, BOOST_INTMAX_C(0x7FFFFFFFD)> R1;
    typedef boost::ratio<3, BOOST_INTMAX_C(0x7FFFFFFFD)> R2;
    typedef boost::ratio_subtract<R1, R2>::type RS;
    std::cout << RS::num << '/' << RS::den << '\n';
    }

    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 0 && R::den == 1, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<-1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -3 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, -2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -3 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<-1, 1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 3 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, -1> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 3 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<56987354, 467584654> R1;
    typedef boost::ratio<544668, 22145> R2;
    typedef boost::ratio_subtract<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -126708206685271LL && R::den == 5177331081415LL, NOTHING, ());
    }
    
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 1, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<-1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, -2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<-1, 1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, -1> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<56987354, 467584654> R1;
    typedef boost::ratio<544668, 22145> R2;
    typedef boost::ratio_multiply<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 15519594064236LL && R::den == 5177331081415LL, NOTHING, ());
    }
    
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 1, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<-1, 2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, -2> R1;
    typedef boost::ratio<1, 1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<-1, 1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<1, 2> R1;
    typedef boost::ratio<1, -1> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == -1 && R::den == 2, NOTHING, ());
    }
    {
    typedef boost::ratio<56987354, 467584654> R1;
    typedef boost::ratio<544668, 22145> R2;
    typedef boost::ratio_divide<R1, R2>::type R;
    BOOST_RATIO_STATIC_ASSERT(R::num == 630992477165LL && R::den == 127339199162436LL, NOTHING, ());
    }

    // test ratio_equal
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_equal<R1, R2>::value), NOTHING, ());
    }

    // test ratio_not_equal

    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_not_equal<R1, R2>::value), NOTHING, ());
    }

    // test ratio_greater

    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater<R1, R2>::value), NOTHING, ());
    }

    // test ratio_greater_equal
    
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_greater_equal<R1, R2>::value), NOTHING, ());
    }

    // test ratio_less

    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFELL, 0x7FFFFFFFFFFFFFFDLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<641981, 1339063> R1;
    typedef boost::ratio<1291640, 2694141LL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1291640, 2694141LL> R1;
    typedef boost::ratio<641981, 1339063> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less<R1, R2>::value), NOTHING, ());
    }

    // test ratio_less_equal
    
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef boost::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef boost::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    
    return 0;
}
