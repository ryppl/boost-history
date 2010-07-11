//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/integer/detail/fusion_ext_includes.hpp>

using namespace boost;
using namespace boost::bitfields;

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

    // at testing
    {
        test_tuple temp;
        temp.get<green>() =3 ;
        BOOST_ASSERT((fusion::at_c<1>(temp) == 3));
    }

    // is_sequence testing
    {
        BOOST_ASSERT(( fusion::traits::is_sequence<test_tuple>() ));
    }
    
    // is_view testing
    {
        BOOST_ASSERT(( !fusion::traits::is_view<test_tuple>() ));
    }

    // category_of testing
    {
        BOOST_MPL_ASSERT((
            is_same<
                fusion::traits::category_of<
                    test_tuple
                >::type, 
                fusion::extension::bft_category
            >
        ));
    }

    // begin testing
    {
        test_tuple temp;
        temp.get<red>() = 4;
        BOOST_ASSERT(( fusion::deref(fusion::begin(temp)) == 4));
    }

    // size testing
    {
        test_tuple temp;
        BOOST_ASSERT(( fusion::size(temp) == 3 ));
    }
    
    // end testing
    {
        test_tuple temp;
        temp.get<salmon>() = 3;
        BOOST_ASSERT(( fusion::deref(fusion::prior(fusion::end(temp))) == 3 ));
    }
    
    // at_key testing
    {
        test_tuple temp;
        temp.get<salmon>() = 3;
        BOOST_ASSERT(( fusion::at_key<salmon>(  temp ) == 3));
        fusion::at_key<salmon>(temp) = 4;
        BOOST_ASSERT(( fusion::at_key<salmon>(  temp ) == 4));
        BOOST_ASSERT(( temp.get<salmon>() == 4));
        BOOST_ASSERT(( fusion::at_key<green>(  temp ) == 0));
    }
}



