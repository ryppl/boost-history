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

// monotonic_clock

// check clock invariants

#include <boost/chrono.hpp>
#include <boost/type_traits/is_same.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

int main()
{
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef boost::chrono::monotonic_clock C;
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<C::rep, C::duration::rep>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<C::period, C::duration::period>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<C::duration, C::time_point::duration>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(C::is_monotonic, NOTHING, ());
#endif
    return 0;
}
