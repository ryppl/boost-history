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

// typedef duration<signed integral type of at least 29 bits, ratio< 60>> minutes;

#include <boost/chrono.hpp>
#include <boost/type_traits.hpp>
#include <limits>

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

int main()
{
    typedef boost::chrono::minutes D;
    typedef D::rep Rep;
    typedef D::period Period;
    BOOST_CHRONO_STATIC_ASSERT(boost::is_signed<Rep>::value, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(boost::is_integral<Rep>::value, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(std::numeric_limits<Rep>::digits >= 28, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<Period, boost::ratio<60> >::value), NOTHING, ());
}
