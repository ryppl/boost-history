//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;

struct red { };
// Fails because storage is specified twice.
typedef bitfield_tuple<storage<int>, storage<unsigned int>, member<int,red,3> > bft;


int main() {
    bft();
    return 0;
}
