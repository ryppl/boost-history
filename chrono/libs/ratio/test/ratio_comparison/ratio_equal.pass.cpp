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

// test ratio_equal

#include <boost/ratio.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

int main()
{
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
}
