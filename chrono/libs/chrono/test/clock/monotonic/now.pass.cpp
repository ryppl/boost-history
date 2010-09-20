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

// monotonic_clock

// static time_point now();

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::monotonic_clock C;
    C::time_point t1 = C::now();
    C::time_point t2 = C::now();
    assert(t2 >= t1);
#endif
    return 0;    
}
