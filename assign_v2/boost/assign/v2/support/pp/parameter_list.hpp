////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#define BOOST_ASSIGN_V2_BOOST_ASSIGN_V2_PARAMETER_LIST_iter(r, data, i, U) \
    BOOST_PP_COMMA_IF(i) \
    U & \
    BOOST_PP_CAT(data, i) \
/**/
#define BOOST_ASSIGN_V2_PARAMETER_LIST(SeqU, data) \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_ASSIGN_V2_BOOST_ASSIGN_V2_PARAMETER_LIST_iter, data, SeqU) \
/**/
#define BOOST_ASSIGN_V2_ARG_LIST(SeqU, data) \
    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(SeqU), data)
/**/
#define BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU) BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(SeqU), typename T)
#define BOOST_ASSIGN_V2_TPL_ARG_LIST(SeqU) BOOST_PP_SEQ_ENUM(SeqU)
//[parameter_list_size
/*Non-override-able constant supporting the definition of functor overloads*/
#define BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE 10
//]
#define BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST BOOST_PP_TUPLE_TO_SEQ(BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE,((T0 const)(T0),(T1 const)(T1),(T2 const)(T2),(T3 const)(T3),(T4 const)(T4),(T5 const)(T5),(T6 const)(T6),(T7 const)(T7),(T8 const)(T8),(T9 const)(T9)))
#endif // BOOST_ASSIGN_V2_PAREMETER_LIST_ER_2010_HPP
