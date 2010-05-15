//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/bit_width.hpp>

// tesitng for bit width.
using namespace boost;



int main() {
    {
        typedef bit_width<char> bw;
        BOOST_ASSERT(( bw::value == 8 ));
        BOOST_ASSERT(( is_same<bw::value_type, unsigned int>::value ));
        BOOST_ASSERT(( is_same<bw::type, bw>::value ));
    }
    return 0;
}
