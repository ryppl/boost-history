//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// duration& operator%=(const rep& rhs)

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::microseconds us(11);
    us %= 3;
    assert(us.count() == 2);
}
