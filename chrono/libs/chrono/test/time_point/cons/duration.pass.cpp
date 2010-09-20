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

// explicit time_point(const duration& d);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    typedef boost::chrono::system_clock Clock;
    typedef boost::chrono::milliseconds Duration;
    {
    boost::chrono::time_point<Clock, Duration> t(Duration(3));
    assert(t.time_since_epoch() == Duration(3));
    }
    {
    boost::chrono::time_point<Clock, Duration> t(boost::chrono::seconds(3));
    assert(t.time_since_epoch() == Duration(3000));
    }
}
