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

/// Testing filter_view

    {
        typedef tuple<int, char, long, X> tuple_type;

        tuple_type t(1, 'x', 987654, X());
        typedef tuple_iterator<0, tuple_type> begin;
        typedef tuple_iterator<4, tuple_type> end;

        typedef tuple_detail::filtered_next<
            begin, end, is_same<_, long> > filter;

        typedef filter::type type;

        BOOST_TEST(*type(t) == 987654);
        std::cout << *type(t) << std::endl;
        std::cout << *filter::get(begin(t)) << std::endl;
        BOOST_TEST(*type(t) == *filter::get(begin(t)));
    }

    {
        typedef tuple<Y, char, long, X, bool, double> tuple_type;

        X x; Y y;
        tuple_type t(y, '@', 987654, x, true, 6.6);
        typedef tuple_view<tuple_type> tuple_view;
        tuple_view tview(t);

        filter_view<tuple_view, not_<is_class<_> > > view(tview);
        std::cout << generate(view) << std::endl;
    }

    return 0;
}

