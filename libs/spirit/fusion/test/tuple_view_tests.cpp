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
        tuple_view<tuple_type> view(t1);

        BOOST_TEST(*(view.begin()) == 1);
        BOOST_TEST(*(next(view.begin())) == 'x');
        BOOST_TEST(*(next(next(view.begin()))) == 3.3);
        BOOST_TEST(*(next(next(next(view.begin())))) == s);

        tuple_type t2(view.begin());
        std::cout << t2 << std::endl;
        BOOST_TEST(t2 == make_tuple(1, 'x', 3.3, s));

        tuple<char, double, char const*> t3(next(view.begin()));
        std::cout << t3 << std::endl;

        BOOST_TEST(t3 == make_tuple('x', 3.3, s));
    }

    {
        char const* s = "Hello";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);
        tuple_view<tuple_type> view(t1);
        std::cout << generate(view.begin(), view.end()) << std::endl;

        BOOST_TEST(generate(view.begin(), view.end()) == t1);
    }

    return 0;
}

