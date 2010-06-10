//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
struct red { };

// this needs to pass because its a basic declaration and I should asume people
// arn't going to be all that happy if things don't work the way the should

// goal of this test is to max out all bits in side of storage using multiple
// members to make sure that if someone wants to do that they can.
typedef bitfield_tuple<
    storage<int>,
    member<int, red, 15u>,
    member<int, red, 17u>
> bft;



int main() {
    bft();
    return 0;
}
