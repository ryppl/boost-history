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

struct X
{
    operator char const*() const
    {
        return "<X-object>";
    }
};

struct Y
{
    operator char const*() const
    {
        return "<Y-object>";
    }
};

int
test_main(int, char*[])
{
    using boost::mpl::_;
    using boost::mpl::not_;
    using boost::is_class;
    using boost::is_same;

    std::cout << tuple_open('[');
    std::cout << tuple_close(']');
    std::cout << tuple_delimiter(", ");

/// Testing filter

    {
        typedef tuple<Y, char, long, X, bool, double> tuple_type;

        X x; Y y;
        tuple_type t(y, '@', 987654, x, true, 6.6);
        typedef tuple_view<tuple_type> tuple_view;
        tuple_view tview(t);

        std::cout << generate(remove_if<not_<is_class<_> > >(tview)) << std::endl;

        BOOST_TEST((generate(remove_if<not_<is_class<_> > >(tview))
            == make_tuple(y, x)));
    }

    {
        typedef tuple<Y, char, long, X, bool, double> tuple_type;

        X x; Y y;
        tuple_type t(y, '@', 987654, x, true, 6.6);
        typedef tuple_view<tuple_type> tuple_view;
        tuple_view tview(t);

        std::cout << generate(remove_if<is_class<_> >(tview)) << std::endl;

        BOOST_TEST((generate(remove_if<is_class<_> >(tview))
            == make_tuple('@', 987654, true, 6.6)));
    }

    return 0;
}

