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

typedef bitfield_tuple<
    storage<std::size_t>,
    member<char,red,4>,
    member<unsigned char, green,5>,
    member<int, salmon, 16>
>       test_tuple_1;


/*
using the bitfield_tuple test_tuple_1 to make "views" out of the proxy reference
types.
*/
int main() {
    {
        typedef element<test_tuple_1,0>::type view_red;
        std::size_t storage = 0;
        view_red    red_v(storage);
        BOOST_TEST(red_v == 0);
        red_v = 3;
        BOOST_TEST(red_v == 3);

    }
   return boost::report_errors();
}
