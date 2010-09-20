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
//   typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
//   operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    {
    boost::chrono::nanoseconds ns1(15);
    boost::chrono::nanoseconds ns2(6);
    boost::chrono::nanoseconds r = ns1 % ns2;
    assert(r.count() == 3);
    }
    {
    boost::chrono::microseconds us1(15);
    boost::chrono::nanoseconds ns2(28);
    boost::chrono::nanoseconds r = us1 % ns2;
    assert(r.count() == 20);
    }
    {
    boost::chrono::duration<int, boost::ratio<3, 5> > s1(6);
    boost::chrono::duration<int, boost::ratio<2, 3> > s2(3);
    boost::chrono::duration<int, boost::ratio<1, 15> > r = s1 % s2;
    assert(r.count() == 24);
    }
}
