// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_INTERFACE_FUNCTION_HPP_INCLUDED
#define BOOST_IDL_INTERFACE_FUNCTION_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/idl/fn_impl.hpp>

#define BOOST_IDL_FN0(name, result)             \
    BOOST_IDL_FN_IMPL(name, result, (0, ()), 0) \
    /**/
#define BOOST_IDL_FN1(name, result, p1)           \
    BOOST_IDL_FN_IMPL(name, result, (1, (p1)), 0) \
    /**/
#define BOOST_IDL_FN2(name, result, p1, p2)           \
    BOOST_IDL_FN_IMPL(name, result, (2, (p1, p2)), 0) \
    /**/
#define BOOST_IDL_FN3(name, result, p1, p2, p3)           \
    BOOST_IDL_FN_IMPL(name, result, (3, (p1, p2, p3)), 0) \
    /**/
#define BOOST_IDL_FN4(name, result, p1, p2, p3, p4)           \
    BOOST_IDL_FN_IMPL(name, result, (4, (p1, p2, p3, p4)), 0) \
    /**/
#define BOOST_IDL_FN5(name, result, p1, p2, p3, p4, p5)           \
    BOOST_IDL_FN_IMPL(name, result, (5, (p1, p2, p3, p4, p5)), 0) \
    /**/
#define BOOST_IDL_FN6(name, result, p1, p2, p3, p4, p5, p6)           \
    BOOST_IDL_FN_IMPL(name, result, (6, (p1, p2, p3, p4, p5, p6)), 0) \
    /**/
#define BOOST_IDL_FN7(name, result, p1, p2, p3, p4, p5, p6, p7)           \
    BOOST_IDL_FN_IMPL(name, result, (7, (p1, p2, p3, p4, p5, p6, p7)), 0) \
    /**/
#define BOOST_IDL_FN8(name, result, p1, p2, p3, p4, p5, p6, p7, p8)           \
    BOOST_IDL_FN_IMPL(name, result, (8, (p1, p2, p3, p4, p5, p6, p7, p8)), 0) \
    /**/
#define BOOST_IDL_FN9(name, result, p1, p2, p3, p4, p5, p6, p7, p8, p9)           \
    BOOST_IDL_FN_IMPL(name, result, (9, (p1, p2, p3, p4, p5, p6, p7, p8, p9)), 0) \
    /**/
#define BOOST_IDL_FN10(name, result, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)           \
    BOOST_IDL_FN_IMPL(name, result, (10, (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)), 0) \
    /**/

#define BOOST_IDL_CONST_FN0(name, result)       \
    BOOST_IDL_FN_IMPL(name, result, (0, ()), 1) \
    /**/
#define BOOST_IDL_CONST_FN1(name, result, p1)     \
    BOOST_IDL_FN_IMPL(name, result, (1, (p1)), 1) \
    /**/
#define BOOST_IDL_CONST_FN2(name, result, p1, p2)     \
    BOOST_IDL_FN_IMPL(name, result, (2, (p1, p2)), 1) \
    /**/
#define BOOST_IDL_CONST_FN3(name, result, p1, p2, p3)     \
    BOOST_IDL_FN_IMPL(name, result, (3, (p1, p2, p3)), 1) \
    /**/
#define BOOST_IDL_CONST_FN4(name, result, p1, p2, p3, p4)     \
    BOOST_IDL_FN_IMPL(name, result, (4, (p1, p2, p3, p4)), 1) \
    /**/
#define BOOST_IDL_CONST_FN5(name, result, p1, p2, p3, p4, p5)     \
    BOOST_IDL_FN_IMPL(name, result, (5, (p1, p2, p3, p4, p5)), 1) \
    /**/
#define BOOST_IDL_CONST_FN6(name, result, p1, p2, p3, p4, p5, p6)     \
    BOOST_IDL_FN_IMPL(name, result, (6, (p1, p2, p3, p4, p5, p6)), 1) \
    /**/
#define BOOST_IDL_CONST_FN7(name, result, p1, p2, p3, p4, p5, p6, p7)     \
    BOOST_IDL_FN_IMPL(name, result, (7, (p1, p2, p3, p4, p5, p6, p7)), 1) \
    /**/
#define BOOST_IDL_CONST_FN8(name, result, p1, p2, p3, p4, p5, p6, p7, p8)     \
    BOOST_IDL_FN_IMPL(name, result, (8, (p1, p2, p3, p4, p5, p6, p7, p8)), 1) \
    /**/
#define BOOST_IDL_CONST_FN9(name, result, p1, p2, p3, p4, p5, p6, p7, p8, p9)     \
    BOOST_IDL_FN_IMPL(name, result, (9, (p1, p2, p3, p4, p5, p6, p7, p8, p9)), 1) \
    /**/
#define BOOST_IDL_CONST_FN10(name, result, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)     \
    BOOST_IDL_FN_IMPL(name, result, (10, (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)), 1) \
    /**/

#endif // #ifndef BOOST_IDL_INTERFACE_FUNCTION_HPP_INCLUDED
