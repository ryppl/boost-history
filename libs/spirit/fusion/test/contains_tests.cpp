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
using boost::mpl::true_;
using boost::mpl::false_;

int
test_main(int, char*[])
{
/// Testing find

    {
        typedef tuple<int, char, int, double> tuple_type;
        tuple_type t(12345, 'x', 678910, 3.36);
        tuple_view<tuple_type> v(t);

        std::cout << contains<char>(v) << std::endl;
        BOOST_TEST(contains<char>(v) == true_());

        std::cout << contains<int>(v) << std::endl;
        BOOST_TEST(contains<int>(v) == true_());

        std::cout << contains<double>(v) << std::endl;
        BOOST_TEST(contains<double>(v) == true_());

        std::cout << contains<short>(v) << std::endl;
        BOOST_TEST(contains<short>(v) == false_());

        std::cout << contains<unsigned>(v) << std::endl;
        BOOST_TEST(contains<unsigned>(v) == false_());
    }

    return 0;
}

