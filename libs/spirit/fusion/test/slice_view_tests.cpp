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

/// Testing slice_view

    {
        char const* s = "Ruby";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);

        {
            typedef tuple_iterator<1, tuple_type> i1t;
            typedef tuple_iterator<3, tuple_type> i3t;

            i1t i1(t1);
            i3t i3(t1);

            slice_view<i1t, i3t> slice(i1, i3);
            std::cout << generate(slice) << std::endl;
            BOOST_TEST(generate(slice) == make_tuple('x', 3.3));
        }

        {
            typedef tuple_iterator<0, tuple_type> i1t;
            typedef tuple_iterator<0, tuple_type> i3t;

            i1t i1(t1);
            i3t i3(t1);

            slice_view<i1t, i3t> slice(i1, i3);
            std::cout << generate(slice) << std::endl;
            BOOST_TEST(generate(slice) == tuple<>());
        }
    }

    return 0;
}

