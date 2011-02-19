////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_args
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#define BOOST_ASSIGN_V2_decl_args_iter(r, data, i, U) \
    BOOST_PP_COMMA_IF(i) \
    U & \
    BOOST_PP_CAT(data, i) \
/**/
#define BOOST_ASSIGN_V2_decl_args(SeqU) \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_ASSIGN_V2_decl_args_iter, _, SeqU) \
/**/
#define BOOST_ASSIGN_V2_args(SeqU) \
    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(SeqU), _)
/**/
#endif
