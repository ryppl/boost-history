//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_EXT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_EXT_ER_2011_HPP
#include <boost/assign/v2/interpreter/csv/csv_ext.hpp>
#include <boost/assign/v2/put/put.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_CSV_PUT_NESTED_ITER(z, N, SeqU)\
    template<\
        int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
        , typename C\
    >\
    typename boost::enable_if_c<I == BOOST_PP_SEQ_SIZE(SeqU)>::type\
    csv_put\
    (\
        C& cont,\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
        csv<I, BOOST_PP_SEQ_ENUM(SeqU)>(\
            put( cont )\
            BOOST_PP_ENUM_TRAILING_PARAMS(\
                BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                _\
            )\
        );\
    }\
    template<\
        typename Options,\
        int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
        , typename C\
    >\
    typename boost::enable_if_c<I == BOOST_PP_SEQ_SIZE(SeqU)>::type\
    csv_put\
    (\
        C& cont,\
        BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
        csv<I, BOOST_PP_SEQ_ENUM(SeqU)>(\
            put( cont ) % Options()\
            BOOST_PP_ENUM_TRAILING_PARAMS(\
                BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                _\
            )\
        );\
    }\
/**/

#define BOOST_ASSIGN_V2_CSV_PUT_ITER(r, SeqU)\
    BOOST_PP_REPEAT_FROM_TO(\
        1,\
        BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),\
        BOOST_ASSIGN_V2_CSV_PUT_NESTED_ITER,\
        SeqU\
    )\
/**/

#define BOOST_ASSIGN_V2_CSV_PUT_CONST_NON_CONST_OVERLOAD(z, I, data)\
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
        BOOST_ASSIGN_V2_CSV_PUT_ITER,\
        BOOST_ASSIGN_V2_CSV_SEQ1(I)\
    )\
/**/

#define BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD(z, I, pos)\
    BOOST_ASSIGN_V2_CSV_PUT_ITER(\
        ~,\
        BOOST_ASSIGN_V2_CSV_SEQ2(pos, I)\
    )\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_ASSIGN_V2_CSV_PUT_CONST_NON_CONST_OVERLOAD,
    ~
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD,
    0
)

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD,
    1
)

#undef BOOST_ASSIGN_V2_CSV_PUT_NESTED_ITER
#undef BOOST_ASSIGN_V2_CSV_PUT_ITER
#undef BOOST_ASSIGN_V2_CSV_PUT_CONST_NON_CONST_OVERLOAD
#undef BOOST_ASSIGN_V2_CSV_PUT_OVERLOAD

}// interpreter_aux
using interpreter_aux::csv_put;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CPP03_CSV_PUT_EXT_ER_2011_HPP
