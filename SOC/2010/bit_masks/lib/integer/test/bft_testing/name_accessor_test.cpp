//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <boost/integer/details/bft/name_lookup.hpp>

using namespace boost;
struct red;
struct green;
struct pink;
struct blue;
struct salmon;


    typedef mpl::vector<
        details::bitfield_element_<
            int,
            red,
            mpl::size_t<0>,
            mpl::size_t<3>
        > 
    >                               test_vect;

int main() {
   

    // tesitng so I can learn to use mpl::find_if
    // for looking up while useing the get<> function in side bitfield_tuple.
    typedef mpl::find_if<
        test_vect,
        details::match_name<
            mpl::_1,
            red
        >
    >::type                         temp_located;

    BOOST_MPL_ASSERT_NOT((
        is_same<
            temp_located,
            mpl::end<
                test_vect
            >::type
        >
    ));
    return 0;
}
