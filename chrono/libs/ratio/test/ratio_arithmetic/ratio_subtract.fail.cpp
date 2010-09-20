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

// test ratio_subtract

#include <boost/ratio.hpp>

typedef boost::ratio<BOOST_INTMAX_C(-0x7FFFFFFFFFFFFFFF), 1> R1;
typedef boost::ratio<1,1> R2;
typedef boost::ratio_subtract<R1, R2>::type RT;
