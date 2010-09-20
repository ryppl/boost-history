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

// template <class ToDuration, class Rep, class Period>
//   ToDuration
//   duration_cast(const duration<Rep, Period>& d);

// ToDuration shall be an instantiation of duration.

#include <boost/chrono.hpp>

int main()
{
    boost::chrono::duration_cast<int>(boost::chrono::milliseconds(3));
    return 0;    
}
