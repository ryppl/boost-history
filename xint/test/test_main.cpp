
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    Previously, this file contained the function to run all of the self-tests
    for the library. Since converting to Boost.Test, it's simply a place to
    declare the auto-generated main function, or a custom one.

    Note that not all functions have a test; all the primitives do, but the rest
    only warrant testing if I'm refactoring them, or if I had problems while
    writing them or in production.
*/

#include <boost/xint/xint.hpp>

#if 1
    #define BOOST_TEST_MAIN
    #define BOOST_TEST_DYN_LINK
    #include <boost/test/unit_test.hpp>
#else
    #include <iostream>

    // For running one-off tests
    int main() {
        using namespace boost::xint::core;
        std::cout << boost::xint::core::pow(19, 1412);
    }
#endif
