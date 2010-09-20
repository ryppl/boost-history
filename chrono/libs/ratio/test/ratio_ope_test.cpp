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
        
    
    return 0;
}
