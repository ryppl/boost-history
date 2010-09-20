//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// Taken from llvm/libcxx/test/utilities/ratio
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test ratio:  The static data members num and den shall have thcommon
//    divisor of the absolute values of N and D:

#include <boost/ratio.hpp>

#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

template <long long N, long long D, long long eN, long long eD>
void test()
{
    BOOST_RATIO_STATIC_ASSERT((boost::ratio<N, D>::num == eN),  NOTHING, ());
    BOOST_RATIO_STATIC_ASSERT((boost::ratio<N, D>::den == eD), NOTHING, ());
}

int main()
{
    test<1, 1, 1, 1>();
    test<1, 10, 1, 10>();
    test<10, 10, 1, 1>();
    test<10, 1, 10, 1>();
    test<12, 4, 3, 1>();
    test<12, -4, -3, 1>();
    test<-12, 4, -3, 1>();
    test<-12, -4, 3, 1>();
    test<4, 12, 1, 3>();
    test<4, -12, -1, 3>();
    test<-4, 12, -1, 3>();
    test<-4, -12, 1, 3>();
    test<222, 333, 2, 3>();
    test<222, -333, -2, 3>();
    test<-222, 333, -2, 3>();
    test<-222, -333, 2, 3>();
    test<0x7FFFFFFFFFFFFFFFLL, 127, 72624976668147841LL, 1>();
    test<-0x7FFFFFFFFFFFFFFFLL, 127, -72624976668147841LL, 1>();
    test<0x7FFFFFFFFFFFFFFFLL, -127, -72624976668147841LL, 1>();
    test<-0x7FFFFFFFFFFFFFFFLL, -127, 72624976668147841LL, 1>();
}
