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

// test ratio_subtract

#include <boost/ratio.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

int main()
{

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
}
