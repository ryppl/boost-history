/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#include <boost/test/minimal.hpp>
#include <boost/spirit/fusion.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/int.hpp>

using namespace boost::fusion;
using boost::mpl::if_;
using boost::mpl::int_;
using boost::is_same;

struct add_ints_only
{
    template <typename T, typename State>
    struct result
    {
        typedef State type;
    };

    template <typename T, typename State>
    State const&
    operator()(T const& x, State const& state) const
    {
        return state;
    }

    int
    operator()(int x, int state) const
    {
        return x + state;
    }
};

struct count_ints
{
    template <typename T, typename CountT>
    struct result
    {
        typedef typename
            if_<
                is_same<T, int>
              , typename boost::mpl::next<CountT>::type
              , CountT
            >::type
        type;
    };

    template <typename T, typename CountT>
    typename result<T, CountT>::type
    operator()(T const&, CountT const&) const
    {
        typedef typename result<T, CountT>::type result;
        return result();
    }
};

int
test_main(int, char*[])
{
/// Testing fold

    {
        typedef tuple<int, char, int, double> tuple_type;
        tuple_type t(12345, 'x', 678910, 3.36);
        tuple_view<tuple_type> v(t);
        int result = fold(v, 0, add_ints_only());
        std::cout << result << std::endl;
        BOOST_TEST(result == 12345+678910);
    }

    {
        typedef tuple<int> tuple_type;
        tuple_type t(12345);
        tuple_view<tuple_type> v(t);

        int n = boost::fusion::fold(v, int_<0>(), count_ints());
        std::cout << n << std::endl;
        BOOST_TEST(n == 1);
    }

    {
        typedef tuple<int, char, int, double, int> tuple_type;
        tuple_type t(12345, 'x', 678910, 3.36, 8756);
        tuple_view<tuple_type> v(t);

        int n = boost::fusion::fold(v, int_<0>(), count_ints());
        std::cout << n << std::endl;
        BOOST_TEST(n == 3);
    }

    return 0;
}

