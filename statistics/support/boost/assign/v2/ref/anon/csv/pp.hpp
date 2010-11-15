//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_PP_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_PP_ER_2010_HPP
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/expr_if.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/fusion/make.hpp>
#include <boost/assign/v2/ref/static_array/alloc/fast_alloc.hpp>
#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>
 
//#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_invoke(z,n,data) \
// ( BOOST_PP_CAT(_,n) ) \
///**/

#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_invoke(z, n, data) \
 result.get_wrapper( n ) = wrapper_( BOOST_PP_CAT(_,n) ); \
/**/

#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(U, N, param, Tag1) \
    template<BOOST_PP_EXPR_IF(param, typename Tag1) \
    	BOOST_PP_COMMA_IF(param) typename T> \
    typename nth_result_of::csv_anon< \
    	N, \
        BOOST_PP_EXPR_IF(param, Tag1) BOOST_PP_COMMA_IF(param) \
        U \
    >::type \
    csv_anon( BOOST_PP_ENUM_PARAMS(N, U& _) ) \
    { \
    	typedef typename nth_result_of::csv_anon< \
    		N, \
        	BOOST_PP_EXPR_IF(param, Tag1) BOOST_PP_COMMA_IF(param) \
        	U \
    	>::type result_; typedef typename result_::wrapper_type wrapper_;\
        result_ result;\
        BOOST_PP_REPEAT( N, BOOST_ASSIGN_V2_REF_CSV_ARRAY_invoke, ~ )\
        return result;\
    } \
/**/

// Old impelementation:
//    	typedef anon_aux::csv_tag2_ tag2_; \
//        return ref::fusion< \
//        	Tag1, \
//            tag2_, \
//            U \
//        >( v2::_nil ) \
//        	BOOST_PP_REPEAT( N, BOOST_ASSIGN_V2_REF_CSV_ARRAY_invoke, ~ ) ; \

#endif

