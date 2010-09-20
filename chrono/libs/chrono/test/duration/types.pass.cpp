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

// Test nested types

// typedef Rep rep;
// typedef Period period;

#include <boost/chrono.hpp>
#include <boost/type_traits.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

int main()
{
    typedef boost::chrono::duration<long, boost::ratio<3, 2> > D;
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<D::rep, long>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<D::period, boost::ratio<3, 2> >::value), NOTHING, ());
}
