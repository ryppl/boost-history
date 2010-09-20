//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// Taken from llvm/libcxx/test/utilities/ratio
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test ratio:  The template argument D shall not be zero

#include <boost/ratio.hpp>
#include <boost/cstdint.hpp>

int main()
{
    const boost::intmax_t t1 = boost::ratio<1, 0>::num;
    (void)t1;
}
