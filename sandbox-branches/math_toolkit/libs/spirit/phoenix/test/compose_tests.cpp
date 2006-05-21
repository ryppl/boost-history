/*=============================================================================
    Copyright (c) 2003-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/minimal.hpp>

#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/core/as_actor.hpp>

using namespace boost::phoenix;
using namespace std;

struct X
{
    template <
        typename Args
      , typename Env
      , typename A0 = void_t
      , typename A1 = void_t
      , typename A2 = void_t
    >
    struct apply
    {
        typedef int type;
    };

    template <typename Args, typename Env, typename A0, typename A1, typename A2>
    static int
    eval(Args const& args, Env const& env, A0& a0, A1& a1, A2& a2)
    {
        return a0.eval(args, env) + a1.eval(args, env) + a2.eval(args, env);
    }
};

int
test_main(int, char*[])
{
    using boost::fusion::get;

    //  testing as_actor
    BOOST_STATIC_ASSERT((boost::is_same<
        as_actor<actor<argument<0> > >::type, actor<argument<0> > >::value));
    BOOST_STATIC_ASSERT((boost::is_same<
        as_actor<int>::type, actor<value<int> > >::value));

    //  testing compose
    char const* s = "Hi";
    int x = 123;

    BOOST_TEST(get<0>(compose<X>(1, arg1, val(1)))() == 1);
    BOOST_TEST(get<1>(compose<X>(1, arg1, val(456)))(s) == s);
    BOOST_TEST(get<2>(compose<X>(1, arg1, val(456)))() == 456);
    BOOST_TEST(compose<X>(9876, arg1, val(456))(x) == 10455);

    //  testing composite sizes
    cout << "sizeof(arg1) is: "
        << sizeof(arg1) << endl;
    cout << "sizeof(compose<X>(arg1)) is: "
        << sizeof(compose<X>(arg1)) << endl;
    cout << "sizeof(compose<X>(1, arg1, val(456))) is: "
        << sizeof(compose<X>(1, arg1, val(456))) << endl;
    cout << "sizeof(compose<X>()) is: "
        << sizeof(compose<X>()) << endl;
    cout << "sizeof(compose<X>('x')) is: "
        << sizeof(compose<X>('x')) << endl;
    cout << "sizeof(compose<X>('x', 3)) is: "
        << sizeof(compose<X>('x', 3)) << endl;
    cout << "sizeof(compose<X>('x', 'y', 3)) is: "
        << sizeof(compose<X>('x', 'y', 3)) << endl;

    return 0;
}
