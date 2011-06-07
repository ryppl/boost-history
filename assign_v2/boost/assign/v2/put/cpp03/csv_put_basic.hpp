//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_BASIC_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_BASIC_ER_2011_HPP
#include <boost/assign/v2/interpreter/csv/csv_basic.hpp>
#include <boost/assign/v2/put/put.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD_UNARY(z, N, is_const)\
    template<typename Options, typename C BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    void csv_put(\
        C& cont\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv(\
            put<Options, C>( cont )\
            BOOST_PP_ENUM_TRAILING_PARAMS(N, _)\
        );\
    }\
    template<typename C BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    void csv_put(\
        C& cont\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv( put( cont ) BOOST_PP_ENUM_TRAILING_PARAMS(N, _) );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD_UNARY,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD_UNARY,
    1
)
#undef BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD_UNARY

}// interpreter_aux
using interpreter_aux::csv_put;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_BASIC_ER_2011_HPP
