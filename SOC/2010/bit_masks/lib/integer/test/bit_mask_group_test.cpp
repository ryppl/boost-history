//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bit_mask_group.hpp>
#include "test_type_list.hpp"


struct name_tag { };
int main() {
    typedef bit_mask_group< named<
        name_tag,low_bits<int,9> >,
        bit_mask<int,3,9>
    > testing_type1;

    BOOST_ASSERT(( sizeof(testing_type1) == 1 ));

    BOOST_ASSERT(( testing_type1().get< 0 >() == low_bits<int,9>() ));
    BOOST_ASSERT(( testing_type1().get< name_tag >() == low_bits<int,9>() ));
    return 0;
}
