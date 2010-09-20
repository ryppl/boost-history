//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// Adapted from llvm/libcxx/test/utilities/chrono
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// template <class Rep1, class Period1, class Rep2, class Period2>
//   typename common_type<Rep1, Rep2>::type
//   operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    {
    boost::chrono::nanoseconds ns1(15);
    boost::chrono::nanoseconds ns2(5);
    assert(ns1 / ns2 == 3);
    }
    {
    boost::chrono::microseconds us1(15);
    boost::chrono::nanoseconds ns2(5);
    assert(us1 / ns2 == 3000);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(30);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(5);
    assert(s1 / s2 == 6);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(30);
    boost::chrono::duration<double, boost::ratio<3, 5> > s2(5);
    assert(s1 / s2 == 20./3);
    }
}
