//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>

using namespace boost;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    member<int, salmon, 16>
>                                       test_tuple;


int main() {
    // bitfield_tuple tag_of testing
    {
        // bitfield_tuple_tag
        typedef fusion::traits::tag_of<test_tuple>::type    tag;
        BOOST_MPL_ASSERT(( is_same< tag, fusion::bitfield_tuple_tag> ));
    }

    // iterator value_of
    //      this is strange to test, because of the construction of the
    // members vector.
    {
        typedef bitfield_tuple_iterator<test_tuple,0>   iter;
        typedef fusion::result_of::value_of<iter>::type value_of_result;
        BOOST_MPL_ASSERT(( is_same< value_of_result::return_type, char> ));
    }
    // Iterator deref
    {
        typedef bitfield_tuple_iterator<test_tuple,0>   Iter;
        test_tuple temp;
        temp.get<red>() = 3;
        Iter it(temp);
        BOOST_ASSERT(( fusion::result_of::deref<Iter>::call(it) == 3 ));
    }
    /*
    bmg_t bmg;

    BOOST_ASSERT(( (*begin(bmg)) == bmg.get<0>() ));
    BOOST_ASSERT(( *next(begin(bmg)) == bmg.get<1>() ));
    BOOST_ASSERT(( *advance_c<2>(begin(bmg)) == bmg.get<2>() ));
    BOOST_ASSERT(( prior(next(begin(bmg))) == begin(bmg) ));
    BOOST_ASSERT(( *prior(end(bmg)) == bmg.get<5>() ));
    BOOST_ASSERT(( at_c<2>(bmg) == bmg.get<2>() ));
    BOOST_ASSERT(( size(bmg) == 6));
    BOOST_ASSERT(( distance(begin(bmg), end(bmg)) == 6 ));

    BOOST_MPL_ASSERT((
        is_same<
            traits::category_of<bmg_t>::type,
            random_access_traversal_tag
        >
    ));
*/
}


