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

// If a program instantiates duration with a duration type for the template
// argument Rep a diagnostic is required.

#include <boost/chrono.hpp>

int main()
{
    typedef boost::chrono::duration<boost::chrono::milliseconds> D;
    D d;
}
