// Copyright David Abrahams 2004.
// Copyright Daniel James 2008.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_MOVE_TEST_GENERIC_TESTS_HEADER)
#define BOOST_MOVE_TEST_GENERIC_TESTS_HEADER

#include <boost/move.hpp>
#include "say.hpp"

namespace test {
    template <class T>
    void tsink(T)
    {
        SAY("in templated rvalue sink");
    }

    template <class X>
    struct generic_tests {

        //
        // Some functions we can use to test the passing of Xs in and out of
        // functions.
        //
     
        static X source()
        {
            return X();
        }

        static X const csource()
        {
            return X();
        }

        static void sink(X)
        {
            SAY("in rvalue sink");
        }

        static void sink2(X&)
        {
            SAY("in non-const lvalue sink2");
        }

        static void sink2(X const&)
        {
            SAY("in const lvalue sink2");
        }

        static void sink3(X&)
        {
            SAY("in non-const lvalue sink3");
        }

        static void move_tests()
        {
            SAY("****** Move Tests *****");
            X::expect_move();

            SAY(" ------ test 1, direct init from rvalue ------- ");
            X z2(source());

            SAY(" ------ test 2, copy init from rvalue ------- ");
            X z4 = X();

            SAY(" ------ test 2.5, construct const ------- ");
            X const z7;

            SAY(" ------ test 3, pass rvalue by-value ------- ");
            sink(source());

            // TODO: This is failing for my noncopyable type. I'm not sure if
            // this is right or wrong.
            //SAY(" ------ test 4, pass const rvalue by-value ------- ");
            //sink(csource());

            SAY(" ------ test 5, pass rvalue by overloaded reference ------- ");
            sink2(source());

            SAY(" ------ test 6, pass const rvalue by overloaded reference ------- ");
            sink2(csource());

            SAY(" ------ test 17, pass rvalue by value to template param ------- ");
            tsink(source());

            SAY(" ------ test 18, direct initialize a const X with an X ------- ");
            typedef X const XC;
            sink2(XC(X()));

            SAY(" ------ test 21, assign from rvalue ------- ");
            z4 = source();

            SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
            X z10(boost::move(z2));
            
            SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
            X z11 = boost::move(z4);
            
            SAY(" ------ test 24, move assign from movable lvalue ------- ");
            z10 = boost::move(z11);
        }

        static void copy_tests()
        {
            SAY("****** Copy Tests *****");

            X::expect_copy();

            SAY(" ------ test 3, copy init from lvalue ------- ");
            X z4;
            X z5 = z4;

            SAY(" ------ test 4, direct init from lvalue ------- ");
            X z6(z4);

            SAY(" ------ test 5, construct const ------- ");
            X const z7;

            SAY(" ------ test 6, copy init from lvalue ------- ");
            X z8 = z7;

            SAY(" ------ test 7, direct init from lvalue ------- ");
            X z9(z7);

            SAY(" ------ test 13, pass lvalue by-value ------- ");
            sink(z5);

            SAY(" ------ test 14, pass const lvalue by-value ------- ");
            sink(z7);

            SAY(" ------ test 15, pass lvalue by-reference ------- ");
            sink2(z4);

            SAY(" ------ test 16, pass const lvalue by const reference ------- ");
            sink2(z7);

            SAY(" ------ test 19, assign from non-const lvalue ------- ");
            z4 = z5;

            SAY(" ------ test 20, assign from const lvalue ------- ");
            z4 = z7;
        }

        static void copy_and_move_tests()
        {
            copy_tests();
            move_tests();
        }
    };
}

#endif
