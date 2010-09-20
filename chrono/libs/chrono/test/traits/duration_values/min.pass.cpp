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

// duration_values::min

#include <boost/chrono.hpp>
#include <limits>
#include <cassert>
#include <iostream>

#include "../../rep.h"

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

int main()
{
    assert(boost::chrono::duration_values<int>::min() ==
           std::numeric_limits<int>::min());
    assert(boost::chrono::duration_values<double>::min() ==
           -std::numeric_limits<double>::max());
    assert(boost::chrono::duration_values<Rep>::min() ==
           std::numeric_limits<Rep>::min());

    //~ std::cout << boost::chrono::duration_values<int>::min() << std::endl;
    //~ std::cout << std::numeric_limits<int>::min() << std::endl;
    //~ std::cout << std::numeric_limits<int>::lowest() << std::endl;
    //~ std::cout << boost::chrono::duration_values<double>::min() << std::endl;
    //~ std::cout << std::numeric_limits<double>::min() << std::endl;
    //~ std::cout << -std::numeric_limits<double>::max() << std::endl;
    //~ std::cout << std::numeric_limits<double>::lowest() << std::endl;
    //~ std::cout << boost::chrono::duration_values<Rep>::max() << std::endl;
    std::cout << (boost::chrono::duration_values<Rep>::min)().data_ << std::endl;
    std::cout << (std::numeric_limits<Rep>::min)().data_ << std::endl;
    std::cout << (std::numeric_limits<Rep>::max)().data_ << std::endl;
           //~ std::numeric_limits<int>::min());
    return 0;
}
