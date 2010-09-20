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

// template <class Duration2>
//   time_point(const time_point<clock, Duration2>& t);

// Duration2 shall be implicitly convertible to duration.

#include <boost/chrono.hpp>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::milliseconds Duration1;
    typedef boost::chrono::microseconds Duration2;
    {
    boost::chrono::time_point<Clock, Duration2> t2(Duration2(3));
    boost::chrono::time_point<Clock, Duration1> t1 = t2;
    }
}
