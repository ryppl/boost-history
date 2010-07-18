//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>
using namespace boost;
using namespace boost::bitfields;
struct rd;

typedef bitfield_tuple<pointer<int, rd> > test_type;

int main() {
    test_type t;
    
    return 0;
}
