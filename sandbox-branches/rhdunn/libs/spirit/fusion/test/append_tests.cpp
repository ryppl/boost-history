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

/// Testing append

    {
        char const* s = "Ruby";
        tuple<int, char, double, char const*> t1(1, 'x', 3.3, s);
        tuple<char, int> t2('y', 1970);
        tuple_view<tuple<int, char, double, char const*> > v1(t1);
        tuple_view<tuple<char, int> > v2(t2);

        std::cout << generate(append(v1, v2)) << std::endl;

        BOOST_TEST((generate(append(v1, v2))
            == make_tuple(1, 'x', 3.3, s, 'y', 1970)));
    }

    {
        char const* s = "Ruby";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);
        tuple_view<tuple_type> v1(t1);

        std::cout << generate(append(v1, single_view<int>(123456))) << std::endl;
        std::cout << generate(append(single_view<int>(123456), v1)) << std::endl;

        BOOST_TEST((generate(append(v1, single_view<int>(123456)))
            == make_tuple(1, 'x', 3.3, s, 123456)));

        BOOST_TEST((generate(append(single_view<int>(123456), v1))
            == make_tuple(123456, 1, 'x', 3.3, s)));
    }

    return 0;
}

