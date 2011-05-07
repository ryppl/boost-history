//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_ER_2011_HPP
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/repetition.hpp>
// This is to ensure that cpp03/csv.hpp compiles as standalone (but it's
// better, instead, to simply include the header below)
#include <boost/assign/v2/interpreter/csv.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename D, typename C, typename F, typename MTag, typename DTag>
    void csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter
    )/*<-*/
    {
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename R
    >
    void invoke(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        as_arg_list_adapter<R> const & adapter
    )/*<-*/
    {
        interpreter.as_arg_list( adapter );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename T
    >
    typename boost::disable_if<
        is_as_arg_list_adapter<
            typename boost::remove_cv<T>::type
        >
    >::type
    invoke(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        T& t
    )/*<-*/
    {
        interpreter( t );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

#define BOOST_ASSIGN_V2_MACRO(z, N, is_const)\
    template<\
        typename D, typename C, typename F, typename MTag, typename DTag,\
        BOOST_PP_ENUM_PARAMS(N, typename T)\
    >\
    void csv(\
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,\
        BOOST_PP_ENUM_BINARY_PARAMS(\
            N, T, BOOST_PP_EXPR_IF(is_const, const) & _\
        )\
    )\
    {\
        invoke( interpreter, _0 );\
        csv(\
            interpreter\
            BOOST_PP_COMMA_IF( BOOST_PP_DEC(N) )\
            BOOST_PP_ENUM_SHIFTED_PARAMS(N, _ )\
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
#undef BOOST_ASSIGN_V2_MACRO

}// interpreter_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_ER_2011_HPP
