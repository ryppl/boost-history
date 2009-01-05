// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move/move.hpp>
#include <boost/static_assert.hpp>
#include "say.hpp"
#include "y.hpp"
#include "./generic_tests.hpp"


//
// Some functions we can use to test the passing of Ys in and out of
// functions.
//
Y y_source()
{
    return Y();
}

Y const y_csource()
{
    return Y();
}

void y_sink(Y)
{
    SAY("in rvalue sink");
}

void y_sink2(Y&)
{
    SAY("in non-const lvalue sink2");
}

void y_sink2(Y const&)
{
    SAY("in const lvalue sink2");
}

void y_sink3(Y&)
{
    SAY("in non-const lvalue sink3");
}

template <class T>
void y_tsink(T)
{
    SAY("in templated rvalue sink");
}

int main()
{
    test::generic_tests<Y>::move_tests();

    // SAY(" ------ test 1, direct init from rvalue ------- ");
    //Y::expect_move();
    Y z2(y_source());

    // SAY(" ------ test 2, copy init from rvalue ------- ");
    //Y::expect_move();
    Y z4 = Y();

    //SAY(" ------ test 3, copy init from lvalue ------- ");
    //Y::expect_copy();
    //Y z5 = z4;

    //SAY(" ------ test 4, direct init from lvalue ------- ");
    //Y::expect_copy();
    //Y z6(z4);

    // SAY(" ------ test 5, construct const ------- ");
    Y const z7;

    //SAY(" ------ test 6, copy init from lvalue ------- ");
    //Y::expect_copy();
    //Y z8 = z7;
    Y z8;

    //SAY(" ------ test 7, direct init from lvalue ------- ");
    //Y::expect_copy();
    //Y z9(z7);
    Y z9;

    //SAY(" ------ test 8, pass rvalue by-value ------- ");
    //Y::expect_move();
    //y_sink(y_source());

    //SAY(" ------ test 9, pass const rvalue by-value ------- ");
    //Y::expect_move();
    //y_sink(y_csource());

    // http://lists.boost.org/Archives/boost/2008/06/138884.php
    //
    // If David Abrahams recalls correctly, when binding a reference to an rvalue,
    // that rvalue must be copyable (and may in fact be copied at the discretion of
    // the compiler). So these two tests only compiler for copyable types.
    //
    // TODO: Look this up properly.
    //SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    //Y::expect_move();
    //y_sink2(y_source());

    //SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    //Y::expect_move();
    //y_sink2(y_csource());

    //SAY(" ------ test 13, pass lvalue by-value ------- ");
    //Y::expect_copy();
    //y_sink(z5);

    //SAY(" ------ test 14, pass const lvalue by-value ------- ");
    //Y::expect_copy();
    //y_sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    //Y::expect_copy();
    y_sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    //Y::expect_copy();
    y_sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    //Y::expect_move();
    y_tsink(y_source());

    //SAY(" ------ test 18, direct initialize a const Y with an Y ------- ");
    //Y::expect_move();
    //typedef Y const YC;
    //y_sink2(YC(Y()));

    //SAY(" ------ test 19, assign from non-const lvalue ------- ");
    //Y::expect_copy();
    //z4 = z5;

    //SAY(" ------ test 20, assign from const lvalue ------- ");
    //Y::expect_copy();
    //z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    //Y::expect_move();
    z4 = y_source();

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<Y>::value);
    //Y::expect_move();
    Y z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    //Y::expect_move();
    Y z11 = boost::move(z9);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    //Y::expect_move();
    z10 = boost::move(z8);

    SAY("----- done, with " << test::suboptimal_copies << " suboptimal copies -----");
    return 0;
}
