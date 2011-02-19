////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_params
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#define BOOST_ASSIGN_V2_decl_params(SeqU) \
    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(SeqU), typename T)\
/**/
#define BOOST_ASSIGN_V2_params(SeqU) BOOST_PP_SEQ_ENUM(SeqU)
#endif
