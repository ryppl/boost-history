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

// template <class Rep2>
//   explicit duration(const Rep2& r);

// Rep2 shall be implicitly convertible to rep

#include <boost/chrono.hpp>

#include "../../rep.h"

int main()
{
    boost::chrono::duration<Rep> d(1);
}
