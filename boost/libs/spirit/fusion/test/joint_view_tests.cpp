/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/io.hpp>
#include <boost/spirit/fusion/sequence/generate.hpp>
#include <boost/spirit/fusion/sequence/equal_to.hpp>
#include <boost/spirit/fusion/sequence/make_tuple.hpp>
#include <boost/spirit/fusion/sequence/joint_view.hpp>
#include <boost/spirit/fusion/sequence/get.hpp>
#include <boost/mpl/vector_c.hpp>

struct X
{
    operator char const*() const
    {
        return "<X-object>";
    }
};

int
test_main(int, char*[])
{
    using namespace boost::fusion;

    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing joint_view
    {
        tuple<int> t1(3);
        tuple<X> t2;
        typedef joint_view<tuple<int>, tuple<X> > view_type;
        view_type view(t1, t2);

        std::cout << view << std::endl;
        BOOST_TEST((view == make_tuple(3, X())));
    }

    {
        tuple<int, char> t1(3, 'x');
        tuple<X> t2;
        typedef joint_view<tuple<int, char>, tuple<X> > view_type;
        view_type view(t1, t2);
        std::cout << view << std::endl;
        BOOST_TEST((view == make_tuple(3, 'x', X())));
        
        *begin(view) = 4;
        BOOST_TEST(get<0>(t1) == 4);
    }

    {
        tuple<int, char> t1(3, 'x');
        tuple<X, int> t2;
        typedef joint_view<tuple<int, char>, tuple<X, int> > view_type;
        view_type view(t1, t2);
        std::cout << view << std::endl;
        BOOST_TEST((view == make_tuple(3, 'x', X(), 0)));
    }

        {
        typedef joint_view<tuple<>, tuple<> > jt_type;
        tuple<> empty;
        jt_type jt(empty, empty);
        std::cout << generate(jt) << std::endl;
        BOOST_TEST(generate(jt) == make_tuple());
    }

    return 0;
}

