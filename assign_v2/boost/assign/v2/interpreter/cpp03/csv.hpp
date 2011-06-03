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
#include <boost/assign/v2/support/config/limit_functor_arity.hpp>
#include <boost/assign/v2/support/config/limit_csv_arity.hpp>
#include <boost/assign/v2/support/config/limit_functor_const_non_const_arity.hpp>
#include <boost/assign/v2/support/pp/parameter_list.hpp>
#include <boost/preprocessor.hpp> // TODO remove
#include <boost/utility/enable_if.hpp>

// This is to ensure that cpp03/csv.hpp compiles as standalone (but it's
// better, instead, to simply include the header below)
#include <boost/assign/v2/interpreter/csv.hpp>

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_EXIT(z, J, data)\
    template<\
        int I, BOOST_PP_ENUM_PARAMS(J, typename T)\
        , typename D, typename C, typename F, typename MTag, typename DTag\
    >\
    typename boost::enable_if_c<\
        I == J,\
        D const&\
    >::type\
    csv\
    (\
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter\
    )\
    {\
        return static_cast<D const&>( interpreter );\
    }\
/**/

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_NESTED_ITER(z, N, SeqU)\
    template<\
        int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
        , typename D, typename C, typename F, typename MTag, typename DTag\
    >\
    typename boost::enable_if_c<\
        I == BOOST_PP_SEQ_SIZE(SeqU),\
        D const&\
    >::type\
    csv\
    (\
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
        interpreter(\
            BOOST_ASSIGN_V2_ARG_LIST(SeqU, 0)\
        );\
        return csv<I, BOOST_PP_SEQ_ENUM(SeqU)>(\
            static_cast<D const&>( interpreter )\
            BOOST_PP_ENUM_TRAILING(\
                BOOST_PP_MUL( BOOST_PP_DEC(N), BOOST_PP_SEQ_SIZE(SeqU) ),\
                BOOST_ASSIGN_V2_CSV_ARG_LIST_ITER,\
                BOOST_PP_SEQ_SIZE(SeqU)\
            )\
        );\
    }\
/**/

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_ITER(r, SeqU)\
    BOOST_PP_REPEAT_FROM_TO(\
        1,\
        BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),\
        BOOST_ASSIGN_V2_INTERPRETER_CSV_NESTED_ITER,\
        SeqU\
    )\
/**/

// CONST_NON_CONST_OVERLOAD

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_CONST_NON_CONST_OVERLOAD(z, I, data)\
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
        BOOST_ASSIGN_V2_INTERPRETER_CSV_ITER,\
        BOOST_ASSIGN_V2_CSV_SEQ1(I)\
    )\
/**/

// OVERLOAD

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD(z, I, pos)\
    BOOST_ASSIGN_V2_INTERPRETER_CSV_ITER(\
        ~,\
        BOOST_ASSIGN_V2_CSV_SEQ2(pos, I)\
    )\
/**/

// OVERLOAD_VARIADIC

#define BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_VARIADIC(z, N, is_const)\
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

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_EXIT,
    ~
)

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_CONST_NON_CONST_OVERLOAD,
    ~
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD,
    0
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD,
    1
)

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
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_VARIADIC,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_VARIADIC,
    1
)

#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_EXIT
#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_NESTED_ITER
#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_ITER
#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_CONST_NON_CONST_OVERLOAD
#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD
#undef BOOST_ASSIGN_V2_INTERPRETER_CSV_OVERLOAD_VARIADIC

}// interpreter_aux

using interpreter_aux::csv;

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_CPP03_CSV_ER_2011_HPP
