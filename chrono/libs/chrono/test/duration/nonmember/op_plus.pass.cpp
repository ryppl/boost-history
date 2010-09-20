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

// <chrono>

// duration

// template <class Rep1, class Period1, class Rep2, class Period2>
//   typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
//   operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    {
    boost::chrono::seconds s1(3);
    boost::chrono::seconds s2(5);
    boost::chrono::seconds r = s1 + s2;
    assert(r.count() == 8);
    }
    {
    boost::chrono::seconds s1(3);
    boost::chrono::microseconds s2(5);
    boost::chrono::microseconds r = s1 + s2;
    assert(r.count() == 3000005);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(3);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(5);
    boost::chrono::duration<int, boost::ratio<1, 15> > r = s1 + s2;
    assert(r.count() == 75);
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(3);
    boost::chrono::duration<double, boost::ratio<3, 5> > s2(5);
    boost::chrono::duration<double, boost::ratio<1, 15> > r = s1 + s2;
    assert(r.count() == 75);
    }
}
