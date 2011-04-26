//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
#include <boost/assign/v2/support/config/limit_csv_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

// Explicit T
#define BOOST_ASSIGN_V2_MACRO(z, N, is_const)\
    template<typename T BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename boost::lazy_disable_if<\
        is_list_option<T0>,\
        result_of::csv_deque<T>\
    >::type\
    csv_deque(BOOST_PP_ENUM_BINARY_PARAMS(\
        N, T, BOOST_PP_EXPR_IF(is_const, const)& _\
    ))\
    {\
        typename result_of::csv_deque<T>::type result;\
        csv( result BOOST_PP_ENUM_TRAILING_PARAMS(N, _) );\
        return result;\
    }\
    template<\
        typename T, typename O\
        BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)\
    >\
    typename boost::lazy_enable_if<\
        is_list_option<O>,\
        result_of::csv_deque_option<O, T>\
    >::type\
    csv_deque(\
        O const& options\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(\
            N, T, BOOST_PP_EXPR_IF(is_const, const)& _\
    ))\
    {\
           typedef typename result_of::csv_deque<T>::type before_op_;\
        typedef typename result_of::csv_deque_option<O, T>::type result_;\
        result_ result = before_op_() % options;\
        csv( result BOOST_PP_ENUM_TRAILING_PARAMS(N, _) );\
        return result;\
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

// Implicit T
#define BOOST_ASSIGN_V2_PAR_LIST(z, N, data) data 
#define BOOST_ASSIGN_V2_MACRO(z, N, is_const)\
    template<typename T>\
    typename boost::lazy_disable_if<\
        is_list_option<T>,\
        result_of::csv_deque<T>\
    >::type\
    csv_deque(\
        BOOST_PP_ENUM_PARAMS(N, T BOOST_PP_EXPR_IF(is_const, const) & _)\
    )\
    {\
        return csv_deque<T, BOOST_PP_ENUM( N, BOOST_ASSIGN_V2_PAR_LIST, T)>(\
            BOOST_PP_ENUM_PARAMS(N, _)\
        );\
    }\
    template<typename O, typename T>\
    typename boost::lazy_enable_if<\
        is_list_option<O>,\
        result_of::csv_deque_option<O, T>\
    >::type\
    csv_deque(\
        O const& options,\
        BOOST_PP_ENUM_PARAMS(N, T BOOST_PP_EXPR_IF(is_const, const) & _)\
    )\
    {\
        return csv_deque<T, O, BOOST_PP_ENUM( N, BOOST_ASSIGN_V2_PAR_LIST, T )>(\
            options,\
            BOOST_PP_ENUM_PARAMS(N, _)\
        );\
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
#undef BOOST_ASSIGN_V2_PAR_LIST
#undef BOOST_ASSIGN_V2_MACRO

}// interpreter_aux
using interpreter_aux::csv_deque;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
