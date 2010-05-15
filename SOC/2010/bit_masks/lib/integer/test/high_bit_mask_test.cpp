//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>

// tesitng for bit width.
using namespace boost;

int main() {
    {
        typedef high_bits<unsigned int, 3> hb;
        BOOST_ASSERT(( hb::value == 3758096384 ));
        BOOST_ASSERT(( is_same<hb::value_type, unsigned int>::value ));
        BOOST_ASSERT(( is_same<hb::type, hb>::value ));
    }
    return 0;
}
