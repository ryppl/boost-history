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
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    member<int, salmon, 16>
>                                       test_tuple;


int main() {

    // testing look up interface
    {
        test_tuple test_1;
        bitfields::get<green>(test_1)      = 15;
        get<red>(test_1)        = 3;
        get<salmon>(test_1)     = 6;
        BOOST_TEST((get<green>(test_1) == 15));
        BOOST_TEST((get<red>(test_1) == 3));
        BOOST_TEST((get<salmon>(test_1) == 6));

        BOOST_TEST( get<salmon>(const_cast<test_tuple const&>(test_1)) == 6 );
        BOOST_TEST( get<green>(const_cast<test_tuple const&>(test_1)) == 15 );
        BOOST_TEST( get<red>(const_cast<test_tuple const&>(test_1)) == 3    );

    }

    // get using an index.
    {
        test_tuple test_2;
        get<0>(test_2) = 3;
        get<1>(test_2) = 14;
        get<2>(test_2) = 6;
        BOOST_TEST(get<0>(test_2) == 3);
        BOOST_TEST(get<1>(test_2) == 14);
        BOOST_TEST(get<2>(test_2) == 6);

        BOOST_TEST(get<2>(const_cast<test_tuple const&>(test_2)) == 6);
        BOOST_TEST(get<0>(const_cast<test_tuple const&>(test_2)) == 3);
        BOOST_TEST(get<1>(const_cast<test_tuple const&>(test_2)) == 14);
    }

    // negative value testing
    {
        test_tuple test_3;
        get<0>(test_3) = -1;
        BOOST_TEST(( get<0>(test_3) == -1 ));
    }

    return boost::report_errors();
}


