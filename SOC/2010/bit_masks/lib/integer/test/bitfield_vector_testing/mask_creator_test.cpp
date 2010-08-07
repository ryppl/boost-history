//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/detail/bitfield_vector/mask_creator.hpp>
#include <boost/detail/lightweight_test.hpp>

using namespace boost::detail;
int main() {
    // testing mask size meta function
    {
        typedef mask_size<
            boost::mpl::size_t<0>,
            boost::mpl::size_t<4>
        >::type test_1;
        BOOST_TEST( test_1::value == 1 );

        typedef mask_size<
            boost::mpl::size_t<6>,
            boost::mpl::size_t<3>
        >::type test_2;
        BOOST_TEST( test_2::value == 2 );

        typedef mask_size<
            boost::mpl::size_t<0>,
            boost::mpl::size_t<50>
        >::type test_3;
        BOOST_TEST( test_3::value == 7 );
    }
    return boost::report_errors();
}
