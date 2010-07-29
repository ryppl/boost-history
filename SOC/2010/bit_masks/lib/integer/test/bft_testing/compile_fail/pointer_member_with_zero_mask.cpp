//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
using namespace boost::bitfields;

struct red { };

// failes because the mask for the pointer has a value of 0.
typedef bitfield_tuple<
    storage<int>,
    pointer<int,red,integral_constant<bitfields::detail::ptr_mask_type, 0> >
> bft;



int main() {
    bft();
    return 0;
}
