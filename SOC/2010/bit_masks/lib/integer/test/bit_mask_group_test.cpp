//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bit_mask_group.hpp>
#include "test_type_list.hpp"

#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/iterator.hpp>

#include <boost/mpl/assert.hpp>


struct name_tag { };
struct name_tag_2 { };

int main() {
    typedef bit_mask_group<
        tagged<
            low_bits_mask<int,9>,
            name_tag
        >,
        bits_mask<int,3,9>
    > testing_type1;

    BOOST_ASSERT(( sizeof(testing_type1) == 1 ));

    BOOST_ASSERT(( testing_type1().get< 0 >() == low_bits_mask<int,9>() ));
    BOOST_ASSERT(( testing_type1().get< name_tag >() == low_bits_mask<int,9>() ));

    BOOST_ASSERT((
        is_same<
            testing_type1::get_by_index<0>::type,
            low_bits_mask<int,9>
        >::value
    ));

    BOOST_ASSERT((
        is_same<
            testing_type1::get_by_name< name_tag >::type,
            low_bits_mask<int,9>
        >::value
    ));

    {

    using namespace boost::fusion;
    typedef boost::bit_mask_group<
        tagged< low_bits_mask< int, 9 >, name_tag >,
        bits_mask< int, 9 >,
        bits_mask<int, 8 >,
        high_bits_mask< int, 3 >,
        integral_mask< int, 0xdeadbeef >,
        tagged< integral_mask< unsigned int, 0xdeadbabe >, name_tag_2 >
    > bmg_t;

    // make sure that this is a sequence and is not a view.
    BOOST_ASSERT(( traits::is_sequence<bmg_t>::value ));
    BOOST_ASSERT(( ! traits::is_view<bmg_t>::value ));


    // testing tag_of for bit_mask_group.
    BOOST_MPL_ASSERT((
        is_same<
            traits::tag_of<bmg_t>::type,
            bit_mask_group_tag
        >
    ));


    // create an instance of this type and then test the rest
    // of the functions which are implemented on this type to make sure they
    // work.
    bmg_t bmg;

    BOOST_ASSERT(( (*begin(bmg)) == bmg.get<0>() ));
    BOOST_ASSERT(( *next(begin(bmg)) == bmg.get<1>() ));
    BOOST_ASSERT(( *advance_c<2>(begin(bmg)) == bmg.get<2>() ));
    BOOST_ASSERT(( prior(next(begin(bmg))) == begin(bmg) ));
    BOOST_ASSERT(( *prior(end(bmg)) == bmg.get<5>() ));
    BOOST_ASSERT(( at_c<2>(bmg) == bmg.get<2>() ));
    BOOST_ASSERT(( size(bmg) == 6));
    BOOST_ASSERT(( distance(begin(bmg), end(bmg)) == 6 ));

#if 0


    BOOST_TEST();
    BOOST_TEST();
    BOOST_TEST();
    BOOST_TEST();
    BOOST_TEST();
    BOOST_TEST();
    BOOST_TEST();

    return boost::report_errors();
}
#endif
    }
    return 0;
}
