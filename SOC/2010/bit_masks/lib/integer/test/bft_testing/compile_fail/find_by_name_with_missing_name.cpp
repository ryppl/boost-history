//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
using namespace boost::bitfields;

struct red { };
struct pink;

// fails because pink isn't a valid name
typedef bitfield_tuple<
    storage<int>,
    member<int,red,2>
> bft;



int main() {
    typedef find_by_element_name<bft,pink>::type t;
    return 0;
}
