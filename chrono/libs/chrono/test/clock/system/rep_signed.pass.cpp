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

// rep should be signed

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    assert((boost::chrono::system_clock::duration::min)() <
           boost::chrono::system_clock::duration::zero());
    return 0;    
}
