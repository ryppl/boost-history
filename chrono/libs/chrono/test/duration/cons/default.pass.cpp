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

// duration() = default;

// Rep must be default initialized, not initialized with 0

#include <boost/chrono.hpp>
#include <cassert>

#include "../../rep.h"

template <class D>
void
test()
{
    D d;
    assert(d.count() == typename D::rep());
}

int main()
{
    test<boost::chrono::duration<Rep> >();
}
