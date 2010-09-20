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

// duration_values::max

#include <boost/chrono.hpp>
#include <boost/type_traits.hpp>
#include <limits>
#include <cassert>

#include "../../rep.h"

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

int main()
{
    assert(boost::chrono::duration_values<int>::max() ==
           std::numeric_limits<int>::max());
    assert(boost::chrono::duration_values<double>::max() ==
           std::numeric_limits<double>::max());
    assert(boost::chrono::duration_values<Rep>::max() ==
           std::numeric_limits<Rep>::max());
}
