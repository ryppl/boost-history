// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move.hpp>
#include <boost/static_assert.hpp>
#include "say.hpp"
#include "y.hpp"

//
// Some functions we can use to test the passing of Ys in and out of
// functions.
//
Y source()
{
    return Y();
}

Y const csource()
{
    return Y();
}

void sink(Y)
{
    SAY("in rvalue sink");
}

void sink2(Y&)
{
    SAY("in non-const lvalue sink2");
}

void sink2(Y const&)
{
    SAY("in const lvalue sink2");
}

void sink3(Y&)
{
    SAY("in non-const lvalue sink3");
}

template <class T>
void tsink(T)
{
    SAY("in templated rvalue sink");
}

int main()
{
    SAY(" ------ test 1, direct init from rvalue ------- ");
    Y::expect_move();
    Y z2(source());

    SAY(" ------ test 2, copy init from rvalue ------- ");
    Y::expect_move();
    Y z4 = Y();

    //SAY(" ------ test 3, copy init from lvalue ------- ");
    //Y::expect_copy();
    //Y z5 = z4;

    //SAY(" ------ test 4, direct init from lvalue ------- ");
    //Y::expect_copy();
    //Y z6(z4);

    SAY(" ------ test 5, construct const ------- ");
    Y const z7;

    //SAY(" ------ test 6, copy init from lvalue ------- ");
    //Y::expect_copy();
    //Y z8 = z7;
    Y z8;

    //SAY(" ------ test 7, direct init from lvalue ------- ");
    //Y::expect_copy();
    //Y z9(z7);
    Y z9;

    SAY(" ------ test 8, pass rvalue by-value ------- ");
    Y::expect_move();
    sink(source());

    SAY(" ------ test 9, pass const rvalue by-value ------- ");
    Y::expect_move();
    sink(csource());

    // http://lists.boost.org/Archives/boost/2008/06/138884.php
    //
    // If David Abrahams recalls correctly, when binding a reference to an rvalue,
    // that rvalue must be copyable (and may in fact be copied at the discretion of
    // the compiler). So these two tests only compiler for copyable types.
    //
    // TODO: Look this up properly.
    //SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    //Y::expect_move();
    //sink2(source());

    //SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    //Y::expect_move();
    //sink2(csource());

    //SAY(" ------ test 13, pass lvalue by-value ------- ");
    //Y::expect_copy();
    //sink(z5);

    //SAY(" ------ test 14, pass const lvalue by-value ------- ");
    //Y::expect_copy();
    //sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    Y::expect_copy();
    sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    Y::expect_copy();
    sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    Y::expect_move();
    tsink(source());

    //SAY(" ------ test 18, direct initialize a const Y with an Y ------- ");
    //Y::expect_move();
    //typedef Y const YC;
    //sink2(YC(Y()));

    //SAY(" ------ test 19, assign from non-const lvalue ------- ");
    //Y::expect_copy();
    //z4 = z5;

    //SAY(" ------ test 20, assign from const lvalue ------- ");
    //Y::expect_copy();
    //z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    Y::expect_move();
    z4 = source();

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<Y>::value);
    Y::expect_move();
    Y z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    Y::expect_move();
    Y z11 = boost::move(z9);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    Y::expect_move();
    z10 = boost::move(z8);
    
    SAY(" ------ test 25, request move construct from non-movable lvalue ------- ");
    BOOST_STATIC_ASSERT(!boost::is_movable<std::string>::value);
    std::string s1("hello");
    std::string s2(boost::move(s1));

    SAY("----- done, with " << Y::suboptimal_copies << " suboptimal copies -----");
    return 0;
}
