//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>


using namespace boost;
using namespace boost::bitfields;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;


    typedef mpl::vector<
        bitfields::detail::bitfield_element<
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
        bitfields::detail::match_name<
            mpl::_1,
            red
        >
    >::type                         temp_located;

    BOOST_TEST((!
        is_same<
            temp_located,
            mpl::end<
                test_vect
            >::type
        >::type::value
    ));
    return boost::report_errors();
}
