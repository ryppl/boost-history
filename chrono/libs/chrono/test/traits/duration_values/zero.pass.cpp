//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//  Adapted by Vicente J. Botet Escriba test Boost.Chrono
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration_values::zero

#include <boost/chrono.hpp>
#include <cassert>

#include "../../rep.h"

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

int main()
{
    assert(boost::chrono::duration_values<int>::zero() == 0);
    assert(boost::chrono::duration_values<Rep>::zero() == 0);
}
