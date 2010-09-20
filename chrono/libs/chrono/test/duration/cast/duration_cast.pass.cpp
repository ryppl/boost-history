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

// duration

// template <class ToDuration, class Rep, class Period>
//   ToDuration
//   duration_cast(const duration<Rep, Period>& d);

#include <boost/chrono.hpp>
#include <boost/type_traits.hpp>
#include <cassert>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

template <class ToDuration, class FromDuration>
void
test(const FromDuration& f, const ToDuration& d)
{
#if defined(BOOST_NO_DECLTYPE)
    typedef BOOST_TYPEOF_TPL(boost::chrono::duration_cast<ToDuration>(f)) R;
#else
    typedef decltype(boost::chrono::duration_cast<ToDuration>(f)) R;
#endif    
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<R, ToDuration>::value), NOTHING, ());
    assert(boost::chrono::duration_cast<ToDuration>(f) == d);
}

int main()
{
    test(boost::chrono::milliseconds(7265000), boost::chrono::hours(2));
    test(boost::chrono::milliseconds(7265000), boost::chrono::minutes(121));
    test(boost::chrono::milliseconds(7265000), boost::chrono::seconds(7265));
    test(boost::chrono::milliseconds(7265000), boost::chrono::milliseconds(7265000));
    test(boost::chrono::milliseconds(7265000), boost::chrono::microseconds(7265000000LL));
    test(boost::chrono::milliseconds(7265000), boost::chrono::nanoseconds(7265000000000LL));
    test(boost::chrono::milliseconds(7265000),
         boost::chrono::duration<double, boost::ratio<3600> >(7265./3600));
    test(boost::chrono::duration<int, boost::ratio<2, 3> >(9),
         boost::chrono::duration<int, boost::ratio<3, 5> >(10));
    return 0;    
}
