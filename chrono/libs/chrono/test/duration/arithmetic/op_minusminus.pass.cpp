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

// duration& operator--();

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::hours h(3);
    boost::chrono::hours& href = --h;
    assert(&href == &h);
    assert(h.count() == 2);
}
