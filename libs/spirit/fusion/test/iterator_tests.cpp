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
    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing the tuple_view

    {
        char const* s = "Hello";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);
        tuple_iterator<0, tuple_type> i(t1);

        BOOST_TEST(*i == 1);
        BOOST_TEST(*(next(i)) == 'x');
        BOOST_TEST(*(next(next(i))) == 3.3);
        BOOST_TEST(*(next(next(next(i)))) == s);

        BOOST_TEST((distance<
            tuple_iterator<0, tuple_type>
          , tuple_iterator<4, tuple_type> >::type::value == 4));
    }

    return 0;
}

