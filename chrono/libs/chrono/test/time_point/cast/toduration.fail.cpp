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

// template <class ToDuration, class Clock, class Duration>
//   time_point<Clock, ToDuration>
//   time_point_cast(const time_point<Clock, Duration>& t);

// ToDuration shall be an instantiation of duration.

#include <boost/chrono.hpp>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::time_point<Clock, boost::chrono::milliseconds> FromTimePoint;
    typedef boost::chrono::time_point<Clock, boost::chrono::minutes> ToTimePoint;
    boost::chrono::time_point_cast<ToTimePoint>(FromTimePoint(boost::chrono::milliseconds(3)));
}
