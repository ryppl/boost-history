//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2011_HPP
#include <boost/assign/v2/support/config/limit_csv_arity.hpp>
#include <boost/assign/v2/support/pp/parameter_list.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_CSV_DEQUE_NESTED_ITER(z, N, SeqU)\
    template<\
        typename T, int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
    >\
    typename boost::lazy_enable_if_c<\
        I == BOOST_PP_SEQ_SIZE(SeqU),\
        result_of::csv_deque<T>\
    >::type\
	csv_deque\
    (\
		BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
		return csv<I, BOOST_PP_SEQ_ENUM(SeqU)>(\
        	deque<T>( _nil )\
            BOOST_PP_ENUM_TRAILING_PARAMS(\
                BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                _\
            )\
        );\
    }\
    template<\
        typename T, typename Options,\
        int I, BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)\
    >\
    typename boost::lazy_enable_if_c<\
        I == BOOST_PP_SEQ_SIZE(SeqU),\
        result_of::csv_deque_option<T, Options>\
    >::type csv_deque\
    (\
		BOOST_ASSIGN_V2_CSV_PARAMETER_LIST(SeqU, N)\
    )\
    {\
		return csv<I, BOOST_PP_SEQ_ENUM(SeqU)>(\
        	deque<T>( _nil ) % Options()\
            BOOST_PP_ENUM_TRAILING_PARAMS(\
                BOOST_PP_MUL( N, BOOST_PP_SEQ_SIZE(SeqU) ),\
                _\
            )\
        );\
    }\
/**/

#define BOOST_ASSIGN_V2_CSV_DEQUE_ITER(r, SeqU)\
	BOOST_PP_REPEAT_FROM_TO(\
    	1,\
    	BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),\
    	BOOST_ASSIGN_V2_CSV_DEQUE_NESTED_ITER,\
    	SeqU\
	)\
/**/

#define BOOST_ASSIGN_V2_CSV_DEQUE_CONST_NON_CONST_OVERLOAD(z, I, data)\
	BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
    	BOOST_ASSIGN_V2_CSV_DEQUE_ITER,\
    	BOOST_ASSIGN_V2_CSV_SEQ1(I)\
	)\
/**/

#define BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD(z, I, pos)\
	BOOST_ASSIGN_V2_CSV_DEQUE_ITER(\
    	~,\
    	BOOST_ASSIGN_V2_CSV_SEQ2(pos, I)\
	)\
/**/

BOOST_PP_REPEAT_FROM_TO(
	1, // TODO zero for empty case
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_CONST_NON_CONST_OVERLOAD,
    ~
)

BOOST_PP_REPEAT_FROM_TO(
	BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD,
    0
)

BOOST_PP_REPEAT_FROM_TO(
	BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD,
    1
)

#undef BOOST_ASSIGN_V2_CSV_DEQUE_NESTED_ITER
#undef BOOST_ASSIGN_V2_CSV_DEQUE_ITER
#undef BOOST_ASSIGN_V2_CSV_DEQUE_CONST_NON_CONST_OVERLOAD
#undef BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD

#define BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC(z, N, is_const)\
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
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC,
    1
)
#undef BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC

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

#define BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC_IMPLICIT(z, N, is_const)\
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
    BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC_IMPLICIT,
    ~
)

#undef BOOST_ASSIGN_V2_MACRO
#undef BOOST_ASSIGN_V2_CSV_DEQUE_OVERLOAD_VARIADIC

}// interpreter_aux
using interpreter_aux::csv_deque;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2011_HPP
