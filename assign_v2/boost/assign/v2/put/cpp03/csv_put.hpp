//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_ER_2010_HPP
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_MACRO(z, N, is_const)\
    template<typename C, typename O, BOOST_PP_ENUM_PARAMS(N, typename T)>\
    typename boost::enable_if<\
        is_option_crtp<O>\
    >::type csv_put(\
        C& cont, O const& options,\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv(\
            make_csv_ready( put( cont ) % options ) \
            , BOOST_PP_ENUM_PARAMS(N, _)\
        );\
    }\
    template<typename C, BOOST_PP_ENUM_PARAMS(N, typename T)>\
    typename boost::disable_if<\
        is_option_crtp<T0>\
    >::type csv_put(\
        C& cont,\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv( make_csv_ready( put( cont ) ), BOOST_PP_ENUM_PARAMS(N, _) );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    1
)
#undef BOOST_ASSIGN_V2_MACRO

}// interpreter_aux
using interpreter_aux::csv_put;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_ER_2010_HPP
