//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_BASIC_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_BASIC_ER_2011_HPP
#include <boost/assign/v2/deque/csv_deque/csv_deque_basic.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC(z, N, is_const)\
    template<typename T, typename Options BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv_deque_option<T, Options>::type\
    csv_deque(\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        return csv(\
            deque<T, Options>( _nil )\
            BOOST_PP_ENUM_TRAILING_PARAMS(N, _)\
        );\
    }\
    template<typename T BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv_deque<T>::type\
    csv_deque(\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        return csv( deque<T>( _nil ) BOOST_PP_ENUM_TRAILING_PARAMS(N, _) );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    0, // case empty
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC,
    1
)
#undef BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC

#define BOOST_ASSIGN_V2_MACRO(z, i, data)\
    T const& BOOST_PP_CAT(_,i)\
/**/

#define BOOST_ASSIGN_V2_REST_PARAMS(N)\
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))\
        BOOST_PP_ENUM_SHIFTED(\
            N,\
            BOOST_ASSIGN_V2_MACRO,\
            ~\
        )\
/**/

#define BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC_IMPLICIT(z, N, is_const)\
    template<typename T, typename Options>\
    typename result_of::csv_deque_option<T, Options>::type\
    csv_deque(\
        T const& _0\
        BOOST_ASSIGN_V2_REST_PARAMS(N)\
    )\
    {\
        typedef typename csv_deque_deduce_value<T>::type t_;\
        return csv(\
            deque<t_, Options>( _nil ), \
            BOOST_PP_ENUM_PARAMS(N, _)\
        );\
    }\
    template<typename T>\
    typename result_of::csv_deque<T>::type\
    csv_deque(\
        T const& _0\
        BOOST_ASSIGN_V2_REST_PARAMS(N)\
    )\
    {\
        typedef typename csv_deque_deduce_value<T>::type t_;\
        return csv( deque<t_>( _nil ), BOOST_PP_ENUM_PARAMS(N, _) );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC_IMPLICIT,
    ~
)

#undef BOOST_ASSIGN_V2_MACRO
#undef BOOST_ASSIGN_V2_REST_PARAMS
#undef BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_BASIC_IMPLICIT

}// interpreter_aux
using interpreter_aux::csv_deque;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_BASIC_ER_2011_HPP
