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

// Period shall be a specialization of ratio, diagnostic required.

#include <boost/chrono.hpp>

template <int N, int D = 1>
class Ratio
{
public:
    static const int num = N;
    static const int den = D;
};

int main()
{
    typedef boost::chrono::duration<int, Ratio<1> > D;
    D d;
}
