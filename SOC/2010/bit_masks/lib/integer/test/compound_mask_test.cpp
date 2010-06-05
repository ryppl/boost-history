//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/compound_mask.hpp>
#include "test_type_list.hpp"


int main() {

    // first testing section for using compond mask.
    {
        // first test.
        typedef compound_mask< high_bits_mask<int,1> , low_bits_mask<int,1> > combo_mask;
        BOOST_ASSERT(( combo_mask() == (high_bits_mask<int,1>() | low_bits_mask<int,1>()) ));

        typedef compound_mask< combo_mask, bits_mask<int, 3, 5> > combo_mask2;
        BOOST_ASSERT(( combo_mask2() == (combo_mask() | bits_mask<int, 3, 5>() ) ));
        BOOST_ASSERT(( combo_mask2() > 0 )) ;
        
        typedef compound_mask<
                    combo_mask2,
                    bits_mask<int,15,10>,
                    integral_mask<int, 6>
                > combo_mask3;

        BOOST_ASSERT((combo_mask3() ==
            ( combo_mask2() | bits_mask<int,15,10>() | integral_mask<int, 6>() )
        ));

    }
    return 0;
}
