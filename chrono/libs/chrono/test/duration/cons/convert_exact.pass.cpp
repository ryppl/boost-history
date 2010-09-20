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

// template <class Rep2, class Period2>
//   duration(const duration<Rep2, Period2>& d);

// exact conversions allowed for integral reps

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::milliseconds ms(1);
    boost::chrono::microseconds us = ms;
    assert(us.count() == 1000);
}
