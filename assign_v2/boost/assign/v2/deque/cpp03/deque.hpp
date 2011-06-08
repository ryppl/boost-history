//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CPP03_DEQUE_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CPP03_DEQUE_ER_2011_HPP
#include <boost/assign/v2/support/config/limit_functor_arity.hpp>
#include <boost/assign/v2/support/config/limit_functor_const_non_const_arity.hpp>
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/deque/fwd.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/deque/deque.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename T>
    typename result_of::deque<T>::type
    deque()
    {
        return deque<T>( v2::_nil )();
    }
/*
    template<typename T, typename Options>
    typename result_of::deque_option<T, Options>::type
    deque()
    {
        return deque<T, Options>( v2::_nil )();
    }
*/

// Overloads for any mixture of const/non-const arguments

#define BOOST_ASSIGN_V2_MACRO1(r, SeqU)\
    template<typename T, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)>\
    typename result_of::deque<T>::type\
    deque( BOOST_ASSIGN_V2_PARAMETER_LIST(SeqU, 0) ){\
        return deque<T>( v2::_nil )(\
            BOOST_ASSIGN_V2_ARG_LIST(SeqU, 0)\
        );\
    }\
/**/
#define BOOST_ASSIGN_V2_MACRO2(z, n, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
    BOOST_ASSIGN_V2_MACRO1,\
    BOOST_PP_SEQ_FIRST_N(BOOST_PP_INC(n), BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST)\
)\
/**/
BOOST_PP_REPEAT(
    BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY,
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

// Overloads for all const and all non-const arguments

#define BOOST_ASSIGN_V2_MACRO(z, N, data)\
\
    template<typename T BOOST_PP_ENUM_TRAILING_PARAMS(N, typename U)>\
    typename result_of::deque<T>::type\
    deque( BOOST_PP_ENUM_BINARY_PARAMS(N, U, &_) ){\
        return deque<T>( v2::_nil )( BOOST_PP_ENUM_PARAMS(N, _) );\
    }\
\
    template<typename T BOOST_PP_ENUM_TRAILING_PARAMS(N, typename U)>\
    typename result_of::deque<T>::type\
    deque( BOOST_PP_ENUM_BINARY_PARAMS(N, const U, &_) ){\
        return deque<T>( v2::_nil )( BOOST_PP_ENUM_PARAMS(N, _) );\
    }\
\
/**/
BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO

}// interpreter_aux

using interpreter_aux::deque;

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CPP03_ER_2011_HPP
