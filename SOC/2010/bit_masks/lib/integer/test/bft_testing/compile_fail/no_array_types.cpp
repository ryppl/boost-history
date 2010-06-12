//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
struct red;
// This is to make sure that array types don't work.
typedef bitfield_tuple< storage<int[3]>, member<int,red,3> > bft;


int main() {
    bft();  // don't for get in order make sure that the template works you 
            // either need to construct it or ask for a type in it!
    return 0;
}
