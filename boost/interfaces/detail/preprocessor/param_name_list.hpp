// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_PARAM_NAME_LIST_HPP_INCLUDED
#define BOOST_IDL_DETAIL_PARAM_NAME_LIST_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/config/has_named_params.hpp>
#include <boost/interfaces/detail/preprocessor/param_name.hpp>
#include <boost/interfaces/detail/preprocessor/param_type.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>

//
// Macro: BOOST_IDL_PARAM_LIST(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: A formal function parameter list of the form
//     BOOST_IDL_PARAM_TYPE(arg0), 
//     ...,
//     BOOST_IDL_PARAM_TYPE(argn)
//
#define BOOST_IDL_PARAM_LIST_IMPL(z, n, args) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_IDL_PARAM_TYPE(BOOST_PP_ARRAY_ELEM(n, args)) \
    /**/
#define BOOST_IDL_PARAM_LIST(args) \
    BOOST_PP_REPEAT( BOOST_PP_ARRAY_SIZE(args), \
                     BOOST_IDL_PARAM_LIST_IMPL, args ) \
    /**/

//
// Macro: BOOST_IDL_NAMED_PARAM_LIST(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: A formal function parameter list of the form
//     BOOST_IDL_PARAM_TYPE(arg0) xxx_0, 
//     ...,
//     BOOST_IDL_PARAM_TYPE(argn) xxx_n
//
#define BOOST_IDL_NAMED_PARAM_LIST_IMPL(z, n, args) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_IDL_PARAM_TYPE(BOOST_PP_ARRAY_ELEM(n, args)) \
    BOOST_PP_CAT(xxx_, n) \
    /**/
#define BOOST_IDL_NAMED_PARAM_LIST(args) \
    BOOST_PP_REPEAT( BOOST_PP_ARRAY_SIZE(args), \
                     BOOST_IDL_NAMED_PARAM_LIST_IMPL, args ) \
    /**/

//
// Macro: BOOST_IDL_PARAM_NAME_LIST(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: 
//
//     static const char* names[] = 
//      { "BOOST_IDL_PARAM_NAME(arg0)",
//        ...,
//        "BOOST_IDL_PARAM_NAME(argn)" };
//
#define BOOST_IDL_PARAM_NAME_LIST_A(args) \
    const char** names = 0; \
    /**/
#define BOOST_IDL_PARAM_NAME_LIST_B_IMPL(z, n, args) \
      BOOST_PP_COMMA_IF(n) \
      BOOST_PP_STRINGIZE(BOOST_IDL_PARAM_NAME( \
      BOOST_PP_ARRAY_ELEM(n, args))) \
    /**/
#define BOOST_IDL_PARAM_NAME_LIST_B(args) \
    static const char* names[] = { \
    BOOST_PP_REPEAT( BOOST_PP_ARRAY_SIZE(args), \
                     BOOST_IDL_PARAM_NAME_LIST_B_IMPL, args ) }; \
    /**/
#define BOOST_IDL_PARAM_NAME_LIST(args) \
    BOOST_PP_IF( BOOST_PP_ARRAY_SIZE(args), \
                 BOOST_IDL_PARAM_NAME_LIST_B, \
                 BOOST_IDL_PARAM_NAME_LIST_A ) (args) \
    /**/

//
// Macro: BOOST_IDL_AUGMENTED_PARAM_LIST(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: the result of adding a parameter xxx_ of type void* to the
//     beginning of BOOST_IDL_NAMED_PARAM_LIST(args).
//        
//
#define BOOST_IDL_AUGMENTED_PARAM_LIST(args) \
    void* xxx_ BOOST_PP_COMMA_IF(BOOST_PP_ARRAY_SIZE(args)) \
    BOOST_IDL_NAMED_PARAM_LIST(args) \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_PARAM_NAME_LIST_HPP_INCLUDED
