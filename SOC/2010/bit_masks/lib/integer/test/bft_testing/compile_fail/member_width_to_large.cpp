//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

using namespace boost;
using namespace boost::bitfields;
// this fails because the type in side member is to large for the return type.
struct pink;

typedef bitfield_tuple<
    storage<int>,
    member<int,pink,33>
> btf_type;

int main() {
    btf_type temp;
    // temp.get<pink>();
    return 0;
}
