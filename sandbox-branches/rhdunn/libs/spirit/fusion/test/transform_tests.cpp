/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>
#include <boost/mpl/range_c.hpp>

using namespace boost::fusion;

struct square
{
    template <typename T>
    struct result
    {
        typedef T type;
    };

    template <typename T>
    T operator()(T x) const
    {
        return x * x;
    }
};

int
test_main(int, char*[])
{
    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing the transform

    {
        typedef boost::mpl::range_c<int, 5, 9> mpl_list1;
        typedef const_value_sequence<mpl_list1> sequence_type;
        sequence_type sequence;

        std::cout << generate(transform(sequence, square())) << std::endl;
        BOOST_TEST((generate(transform(sequence, square()))
            == make_tuple(25, 36, 49, 64)));
    }

    return 0;
}

