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

// static constexpr duration max();

#include <boost/chrono.hpp>
#include <limits>
#include <cassert>

#include "../../rep.h"

template <class D>
void test()
{
    typedef typename D::rep Rep;
    Rep max_rep = boost::chrono::duration_values<Rep>::max();
    assert(D::max().count() == max_rep);
}

int main()
{
    test<boost::chrono::duration<int> >();
    test<boost::chrono::duration<Rep> >();
}
