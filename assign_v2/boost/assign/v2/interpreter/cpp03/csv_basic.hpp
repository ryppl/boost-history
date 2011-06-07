//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_BASIC_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_BASIC_ER_2011_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/support/config/limit_csv_arity.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/punctuation.hpp>
#include <boost/preprocessor/repetition.hpp>

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_BASIC(z, N, is_const)\
    template<\
        BOOST_PP_ENUM_PARAMS(N, typename T)\
        BOOST_PP_COMMA_IF(N)\
        typename D, typename C, typename F, typename MTag, typename DTag\
    >\
    D const& csv(\
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,\
        BOOST_PP_ENUM_BINARY_PARAMS(\
            N, T, BOOST_PP_EXPR_IF(is_const, const) & _\
        )\
    )\
    {\
        interpreter( _0 );\
        return csv(\
            interpreter\
            BOOST_PP_COMMA_IF( BOOST_PP_DEC(N) )\
            BOOST_PP_ENUM_SHIFTED_PARAMS(N, _ )\
        );\
    }\
/**/

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename D, typename C, typename F, typename MTag, typename DTag>
    D const& csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter
    )
    {
        return static_cast<D const&>( interpreter );
    }

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_BASIC,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_BASIC,
    1
)

#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_BASIC

}// interpreter_aux

using interpreter_aux::csv;

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_BASIC_ER_2011_HPP
