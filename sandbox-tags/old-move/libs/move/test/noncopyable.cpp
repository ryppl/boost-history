// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move/move.hpp>
#include <boost/static_assert.hpp>
#include "say.hpp"
#include "noncopyable.hpp"
#include "./generic_tests.hpp"


//
// Some functions we can use to test the passing of Ys in and out of
// functions.
//
noncopyable y_source()
{
    return noncopyable();
}

noncopyable const y_csource()
{
    return noncopyable();
}

void y_sink(noncopyable)
{
    SAY("in rvalue sink");
}

void y_sink2(noncopyable&)
{
    SAY("in non-const lvalue sink2");
}

void y_sink2(noncopyable const&)
{
    SAY("in const lvalue sink2");
}

void y_sink3(noncopyable&)
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
    test::generic_tests<noncopyable>::move_tests();

    // SAY(" ------ test 1, direct init from rvalue ------- ");
    //noncopyable::expect_move();
    noncopyable z2(y_source());

    // SAY(" ------ test 2, copy init from rvalue ------- ");
    //noncopyable::expect_move();
    noncopyable z4 = noncopyable();

    //SAY(" ------ test 3, copy init from lvalue ------- ");
    //noncopyable::expect_copy();
    //noncopyable z5 = z4;

    //SAY(" ------ test 4, direct init from lvalue ------- ");
    //noncopyable::expect_copy();
    //noncopyable z6(z4);

    // SAY(" ------ test 5, construct const ------- ");
    noncopyable const z7;

    //SAY(" ------ test 6, copy init from lvalue ------- ");
    //noncopyable::expect_copy();
    //noncopyable z8 = z7;
    noncopyable z8;

    //SAY(" ------ test 7, direct init from lvalue ------- ");
    //noncopyable::expect_copy();
    //noncopyable z9(z7);
    noncopyable z9;

    //SAY(" ------ test 8, pass rvalue by-value ------- ");
    //noncopyable::expect_move();
    //y_sink(y_source());

    //SAY(" ------ test 9, pass const rvalue by-value ------- ");
    //noncopyable::expect_move();
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
    //noncopyable::expect_copy();
    //y_sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    //noncopyable::expect_copy();
    y_sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    //noncopyable::expect_copy();
    y_sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    //noncopyable::expect_move();
    y_tsink(y_source());

    //SAY(" ------ test 18, direct initialize a const noncopyable with an noncopyable ------- ");
    //noncopyable::expect_move();
    //typedef noncopyable const YC;
    //y_sink2(YC(noncopyable()));

    //SAY(" ------ test 19, assign from non-const lvalue ------- ");
    //noncopyable::expect_copy();
    //z4 = z5;

    //SAY(" ------ test 20, assign from const lvalue ------- ");
    //noncopyable::expect_copy();
    //z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    //noncopyable::expect_move();
    z4 = y_source();

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<noncopyable>::value);
    //noncopyable::expect_move();
    noncopyable z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    //noncopyable::expect_move();
    noncopyable z11 = boost::move(z9);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    //noncopyable::expect_move();
    z10 = boost::move(z8);

    SAY("----- done, with " << test::suboptimal_copies << " suboptimal copies -----");
    return 0;
}
