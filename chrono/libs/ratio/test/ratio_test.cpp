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

// typedefs
BOOST_RATIO_STATIC_ASSERT(boost::atto::num == 1 && boost::atto::den == 1000000000000000000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::femto::num == 1 && boost::femto::den == 1000000000000000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::pico::num == 1 && boost::pico::den == 1000000000000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::nano::num == 1 && boost::nano::den == 1000000000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::micro::num == 1 && boost::micro::den == 1000000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::milli::num == 1 && boost::milli::den == 1000ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::centi::num == 1 && boost::centi::den == 100ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::deci::num == 1 && boost::deci::den == 10ULL, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::deca::num == 10ULL && boost::deca::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::hecto::num == 100ULL && boost::hecto::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::kilo::num == 1000ULL && boost::kilo::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::mega::num == 1000000ULL && boost::mega::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::giga::num == 1000000000ULL && boost::giga::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::tera::num == 1000000000000ULL && boost::tera::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::peta::num == 1000000000000000ULL && boost::peta::den == 1, NOTHING, ());
BOOST_RATIO_STATIC_ASSERT(boost::exa::num == 1000000000000000000ULL && boost::exa::den == 1, NOTHING, ());


template <long long N, long long D, long long eN, long long eD>
void test_simplification()
{
    BOOST_RATIO_STATIC_ASSERT((boost::ratio<N, D>::num == eN),  NOTHING, ());
    BOOST_RATIO_STATIC_ASSERT((boost::ratio<N, D>::den == eD), NOTHING, ());
}

int main()
{

    test_simplification<1, 1, 1, 1>();
    test_simplification<1, 10, 1, 10>();
    test_simplification<10, 10, 1, 1>();
    test_simplification<10, 1, 10, 1>();
    test_simplification<12, 4, 3, 1>();
    test_simplification<12, -4, -3, 1>();
    test_simplification<-12, 4, -3, 1>();
    test_simplification<-12, -4, 3, 1>();
    test_simplification<4, 12, 1, 3>();
    test_simplification<4, -12, -1, 3>();
    test_simplification<-4, 12, -1, 3>();
    test_simplification<-4, -12, 1, 3>();
    test_simplification<222, 333, 2, 3>();
    test_simplification<222, -333, -2, 3>();
    test_simplification<-222, 333, -2, 3>();
    test_simplification<-222, -333, 2, 3>();
    test_simplification<0x7FFFFFFFFFFFFFFFLL, 127, 72624976668147841LL, 1>();
    test_simplification<-0x7FFFFFFFFFFFFFFFLL, 127, -72624976668147841LL, 1>();
    test_simplification<0x7FFFFFFFFFFFFFFFLL, -127, -72624976668147841LL, 1>();
    test_simplification<-0x7FFFFFFFFFFFFFFFLL, -127, 72624976668147841LL, 1>();
    
    typedef boost::ratio<8, BOOST_INTMAX_C(0x7FFFFFFFD)> R1;
    typedef boost::ratio<3, BOOST_INTMAX_C(0x7FFFFFFFD)> R2;
    typedef boost::ratio_subtract<R1, R2>::type RS;
    std::cout << RS::num << '/' << RS::den << '\n';
    
    
  return 0;
}
