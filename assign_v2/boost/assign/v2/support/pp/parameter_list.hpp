////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2011 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2011_HPP
#define BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2011_HPP
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/punctuation.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/seq.hpp>

#define BOOST_ASSIGN_V2_ARG_LIST_ITER( z, n, shift )\
    BOOST_PP_CAT(\
        _,\
        BOOST_PP_ADD(\
            shift,\
            n\
        )\
    )\
/**/

#define BOOST_ASSIGN_V2_PARAMETER_LIST_ITER( r, shift, i, U )\
    BOOST_PP_COMMA_IF(i)\
    U &\
    BOOST_ASSIGN_V2_ARG_LIST_ITER(~, i, shift)\
/**/

#define BOOST_ASSIGN_V2_PARAMETER_LIST(SeqU, shift)\
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_ASSIGN_V2_PARAMETER_LIST_ITER, shift, SeqU)\
/**/

#define BOOST_ASSIGN_V2_ARG_LIST(SeqU, shift)\
    BOOST_PP_ENUM(\
        BOOST_PP_SEQ_SIZE(SeqU),\
        BOOST_ASSIGN_V2_ARG_LIST_ITER,\
        shift\
    )\
/**/

#define BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
    BOOST_PP_ENUM_PARAMS(\
        BOOST_PP_SEQ_SIZE(SeqU),\
        typename T\
    )\
/**/

#define BOOST_ASSIGN_V2_TPL_ARG_LIST(SeqU) BOOST_PP_SEQ_ENUM(SeqU)
//[parameter_list_size
// Non-override-able constant supporting the definition of functor overloads
// under C++03
#define BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE 100
//]

#define BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST_ITER(z, n, data)\
    (\
        ( BOOST_PP_CAT(T, n) const)( BOOST_PP_CAT(T, n) )\
    )\
/**/

#define BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST\
    BOOST_PP_REPEAT(\
        BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE,\
        BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST_ITER,\
        ~\
    )\
/**/

// CSV

// PARAMETER_LIST

#define BOOST_ASSIGN_V2_CSV_PARAMETER_LIST_ITER(z, n, SeqU)\
    BOOST_ASSIGN_V2_PARAMETER_LIST(\
        SeqU,\
        BOOST_PP_MUL(\
            n,\
            BOOST_PP_SEQ_SIZE(SeqU)\
        )\
    )\
/**/

#define BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    BOOST_PP_ENUM(\
        N,\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST_ITER,\
        SeqU\
    )\
/**/

// ARG_LIST

#define BOOST_ASSIGN_V2_CSV_ARG_LIST_ITER(z, i, N)\
    BOOST_PP_CAT(\
        _,\
        BOOST_PP_ADD( N, i)\
    )\
/**/

// MISC

#define BOOST_ASSIGN_V2_CSV_SEQ1(I)\
    BOOST_PP_SEQ_FIRST_N(\
        I,\
        BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST\
    )\
/**/

#define BOOST_ASSIGN_V2_CSV_SEQ_ELEM(r, data, elem)\
    (BOOST_PP_SEQ_ELEM(data, elem))\
/**/

#define BOOST_ASSIGN_V2_CSV_SEQ2(pos, I)\
    BOOST_PP_SEQ_FOR_EACH(\
        BOOST_ASSIGN_V2_CSV_SEQ_ELEM,\
        pos,\
        BOOST_ASSIGN_V2_CSV_SEQ1(I)\
    )\
/**/

#endif // BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2011_HPP
