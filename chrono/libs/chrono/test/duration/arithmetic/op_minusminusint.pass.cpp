//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// duration operator--(int);

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    boost::chrono::hours h(3);
    boost::chrono::hours h2 = h--;
    assert(h.count() == 2);
    assert(h2.count() == 3);
}
