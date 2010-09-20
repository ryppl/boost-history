//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// duration& operator%=(const duration& rhs)

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::microseconds us(11);
    boost::chrono::microseconds us2(3);
    us %= us2;
    assert(us.count() == 2);
    us %= boost::chrono::milliseconds(3);
    assert(us.count() == 2);
}
