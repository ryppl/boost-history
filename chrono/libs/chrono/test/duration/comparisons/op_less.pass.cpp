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
//   bool
//   operator< (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

// template <class Rep1, class Period1, class Rep2, class Period2>
//   bool
//   operator> (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

// template <class Rep1, class Period1, class Rep2, class Period2>
//   bool
//   operator<=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

// template <class Rep1, class Period1, class Rep2, class Period2>
//   bool
//   operator>=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    {
    boost::chrono::seconds s1(3);
    boost::chrono::seconds s2(3);
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert( (s1 >= s2));
    }
    {
    boost::chrono::seconds s1(3);
    boost::chrono::seconds s2(4);
    assert( (s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert(!(s1 >= s2));
    }
    {
    boost::chrono::milliseconds s1(3);
    boost::chrono::microseconds s2(3000);
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert( (s1 >= s2));
    }
    {
    boost::chrono::milliseconds s1(3);
    boost::chrono::microseconds s2(4000);
    assert( (s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert(!(s1 >= s2));
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(9);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(10);
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert( (s1 >= s2));
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(10);
    boost::chrono::duration<int, boost::ratio<3, 5> > s2(9);
    assert(!(s1 < s2));
    assert( (s1 > s2));
    assert(!(s1 <= s2));
    assert( (s1 >= s2));
    }
    {
    boost::chrono::duration<int, boost::ratio<2, 3> > s1(9);
    boost::chrono::duration<double, boost::ratio<3, 5> > s2(10);
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert( (s1 <= s2));
    assert( (s1 >= s2));
    }
}
