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

/// Testing push_front

    {
        char const* s = "Ruby";
        typedef tuple<int, char, double, char const*> tuple_type;
        tuple_type t1(1, 'x', 3.3, s);
        tuple_view<tuple_type> v1(t1);

        {
            std::cout << generate(push_front(v1, 123456)) << std::endl;
            BOOST_TEST((generate(push_front(v1, 123456))
                == make_tuple(123456, 1, 'x', 3.3, s)));
        }

        {
            std::cout << generate(push_front(v1, "lively")) << std::endl;
            BOOST_TEST((generate(push_front(v1, "lively"))
                == make_tuple(std::string("lively"), 1, 'x', 3.3, s)));
        }
    }

    return 0;
}

