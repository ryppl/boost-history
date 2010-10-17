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

// system_clock

// time_t to_time_t(const time_point& t);

#include <boost/chrono.hpp>
#include <ctime>

int main()
{
    typedef boost::chrono::system_clock C;
    std::time_t t1 = C::to_time_t(C::now());
    (void)t1;
    return 0;
}
