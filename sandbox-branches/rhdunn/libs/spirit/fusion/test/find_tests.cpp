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
/// Testing find

    {
        typedef tuple<int, char, int, double> tuple_type;
        tuple_type t(12345, 'x', 678910, 3.36);
        tuple_view<tuple_type> v(t);

        std::cout << *find<char>(v) << std::endl;
        BOOST_TEST(*find<char>(v) == 'x');

        std::cout << *find<int>(v) << std::endl;
        BOOST_TEST(*find<int>(v) == 12345);

        std::cout << *find<double>(v) << std::endl;
        BOOST_TEST(*find<double>(v) == 3.36);
    }

    return 0;
}

