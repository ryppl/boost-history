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

// static constexpr duration min();

#include <boost/chrono.hpp>
#include <boost/limits.hpp>
#include <cassert>

#include "../../rep.h"

template <class D>
void test()
{
    typedef typename D::rep Rep;
    Rep min_rep = boost::chrono::duration_values<Rep>::min();
    assert(D::min().count() == min_rep);
}

int main()
{
    test<boost::chrono::duration<int> >();
    test<boost::chrono::duration<Rep> >();
}
