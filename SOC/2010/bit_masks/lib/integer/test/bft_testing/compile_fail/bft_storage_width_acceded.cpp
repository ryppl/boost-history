//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
struct red { };

// Fails because the members exced the number of bits inside of
// the storage type.
typedef bitfield_tuple<
    storage<int>,
    member<int, red, 33u>
> bft;



int main() {
    bft();
    return 0;
}
