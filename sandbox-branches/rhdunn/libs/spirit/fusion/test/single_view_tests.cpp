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

/// Testing single_view

    {
        single_view<int> view1(3);
        std::cout << generate(view1) << std::endl;

        single_view<X> view2;
        std::cout << generate(view2) << std::endl;
    }

    {
        // VC7.1 BUG!!!
        tuple<char const(&)[17]> t(
            single_iterator<char const(&)[17]>("VC7.1 BUG-FIX!!!"));
        std::cout << t << std::endl;
    }

    return 0;
}

