//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// duration

// duration operator-() const;

#include <boost/chrono.hpp>
#include <cassert>

int main()
{
    const boost::chrono::minutes m(3);
    boost::chrono::minutes m2 = -m;
    assert(m2.count() == -m.count());
}
