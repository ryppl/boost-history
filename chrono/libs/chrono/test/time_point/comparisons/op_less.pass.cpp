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

// time_point

// template <class Clock, class Duration1, class Duration2>
//   bool
//   operator< (const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs);

// template <class Clock, class Duration1, class Duration2>
//   bool
//   operator> (const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs);

// template <class Clock, class Duration1, class Duration2>
//   bool
//   operator<=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs);

// template <class Clock, class Duration1, class Duration2>
//   bool
//   operator>=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::milliseconds Duration1;
    typedef boost::chrono::microseconds Duration2;
    typedef boost::chrono::time_point<Clock, Duration1> T1;
    typedef boost::chrono::time_point<Clock, Duration2> T2;

    {
    T1 t1(Duration1(3));
    T1 t2(Duration1(3));
    assert(!(t1 <  t2));
    assert(!(t1 >  t2));
    assert( (t1 <= t2));
    assert( (t1 >= t2));
    }
    {
    T1 t1(Duration1(3));
    T1 t2(Duration1(4));
    assert( (t1 <  t2));
    assert(!(t1 >  t2));
    assert( (t1 <= t2));
    assert(!(t1 >= t2));
    }
    {
    T1 t1(Duration1(3));
    T2 t2(Duration2(3000));
    assert(!(t1 <  t2));
    assert(!(t1 >  t2));
    assert( (t1 <= t2));
    assert( (t1 >= t2));
    }
    {
    T1 t1(Duration1(3));
    T2 t2(Duration2(3001));
    assert( (t1 <  t2));
    assert(!(t1 >  t2));
    assert( (t1 <= t2));
    assert(!(t1 >= t2));
    }
}
