/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>

using namespace boost::fusion;

int
test_main(int, char*[])
{
/// Testing count

    {
        typedef tuple<int> tuple_type;
        tuple_type t(12345);
        tuple_view<tuple_type> v(t);

        int n = boost::fusion::count<int>(v);
        std::cout << n << std::endl;
        BOOST_TEST(n == 1);
    }

    {
        typedef tuple<int, char, int, double, int> tuple_type;
        tuple_type t(12345, 'x', 678910, 3.36, 8756);
        tuple_view<tuple_type> v(t);

        int n = boost::fusion::count<int>(v);
        std::cout << n << std::endl;
        BOOST_TEST(n == 3);
    }

    return 0;
}

