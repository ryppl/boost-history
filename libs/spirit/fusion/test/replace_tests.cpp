/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>
#include <string>

using namespace boost::fusion;

int
test_main(int, char*[])
{
    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing replace

    {
        char const* s = "Ruby";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);
        tuple_view<tuple_type> v1(t1);
        tuple_iterator<2, tuple_type> pos(t1);

        std::cout << generate(replace(v1, pos, 123456)) << std::endl;
        BOOST_TEST((generate(replace(v1, pos, 123456))
            == make_tuple(1, 'x', 123456, s)));

        std::cout << generate(replace(v1, v1.begin(), "happy")) << std::endl;
        BOOST_TEST((generate(replace(v1, v1.begin(), "happy"))
            == make_tuple(std::string("happy"), 'x', 3.3, s)));
    }

    return 0;
}

