//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// duration& operator+=(const duration& d);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::seconds s(3);
    s += boost::chrono::seconds(2);
    assert(s.count() == 5);
    s += boost::chrono::minutes(2);
    assert(s.count() == 125);
}
