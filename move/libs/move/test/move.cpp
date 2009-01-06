// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move/move.hpp>
#include <boost/static_assert.hpp>
#include "say.hpp"
#include "movable.hpp"

//
// Some functions we can use to test the passing of Xs in and out of
// functions.
//
movable source()
{
    return movable();
}

movable const csource()
{
    return movable();
}

void sink(movable)
{
    SAY("in rvalue sink");
}

void sink2(movable&)
{
    SAY("in non-const lvalue sink2");
}

void sink2(movable const&)
{
    SAY("in const lvalue sink2");
}

void sink3(movable&)
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
    //movable::expect_move();
    movable z2(source());

    SAY(" ------ test 2, copy init from rvalue ------- ");
    //movable::expect_move();
    movable z4 = movable();

    SAY(" ------ test 3, copy init from lvalue ------- ");
    //movable::expect_copy();
    movable z5 = z4;

    SAY(" ------ test 4, direct init from lvalue ------- ");
    //movable::expect_copy();
    movable z6(z4);

    SAY(" ------ test 5, construct const ------- ");
    movable const z7;

    SAY(" ------ test 6, copy init from lvalue ------- ");
    //movable::expect_copy();
    movable z8 = z7;

    SAY(" ------ test 7, direct init from lvalue ------- ");
    //movable::expect_copy();
    movable z9(z7);

    SAY(" ------ test 8, pass rvalue by-value ------- ");
    //movable::expect_move();
    sink(source());

    SAY(" ------ test 9, pass const rvalue by-value ------- ");
    //movable::expect_move();
    sink(csource());

    SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    //movable::expect_move();
    sink2(source());

    SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    //movable::expect_move();
    sink2(csource());

    SAY(" ------ test 13, pass lvalue by-value ------- ");
    //movable::expect_copy();
    sink(z5);

    SAY(" ------ test 14, pass const lvalue by-value ------- ");
    //movable::expect_copy();
    sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    //movable::expect_copy();
    sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    //movable::expect_copy();
    sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    //movable::expect_move();
    tsink(source());

    SAY(" ------ test 18, direct initialize a const movable with an movable ------- ");
    //movable::expect_move();
    typedef movable const XC;
    sink2(XC(movable()));

    SAY(" ------ test 19, assign from non-const lvalue ------- ");
    //movable::expect_copy();
    z4 = z5;

    SAY(" ------ test 20, assign from const lvalue ------- ");
    //movable::expect_copy();
    z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    //movable::expect_move();
    z4 = source();

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<movable>::value);
    //movable::expect_move();
    movable z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    //movable::expect_move();
    movable z11 = boost::move(z9);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    //movable::expect_move();
    z10 = boost::move(z8);
    
    SAY(" ------ test 25, request move construct from non-movable lvalue ------- ");
    BOOST_STATIC_ASSERT(!boost::is_movable<std::string>::value);
    std::string s1("hello");
    std::string s2(boost::move(s1));

    SAY("----- done, with " << test::suboptimal_copies << " suboptimal copies -----");
    return 0;
}
