/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>
#include <boost/mpl/list_c.hpp>

using namespace boost::fusion;

int
test_main(int, char*[])
{
    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing const_value_sequence

    {
        typedef boost::mpl::list_c<int, 4, 1, 3, 2> mpl_list1;
        typedef boost::mpl::begin<mpl_list1>::type begin_type;
        typedef boost::mpl::end<mpl_list1>::type end_type;
        typedef tuple<int, int, int, int> tuple_type;

        const_value_iterator<begin_type> iter;
        tuple_type t1(iter);
        std::cout << t1 << std::endl;

        BOOST_TEST(t1 == tuple_type(4, 1, 3, 2));
    }

    {
        typedef boost::mpl::list_c<char, 'K', 'I', 'M', 'P', 'O'> mpl_list1;
        const_value_sequence<mpl_list1> view;
        std::cout << generate(view) << std::endl;

        BOOST_TEST((generate(view) == make_tuple('K', 'I', 'M', 'P', 'O')));
    }

    return 0;
}

