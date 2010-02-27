//////////////////////////////////////////////////////////////////////////////
// assign::detail::csv.hpp                                           		//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_DETAIL_CSV_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_DETAIL_CSV_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Warning : currently buggy

#define BOOST_ASSIGN_LIST_OF_CSV_tmp(z,n,unused) (BOOST_PP_CAT(_,n))
#define BOOST_ASSIGN_LIST_OF_CSV_rec(fun,N) 								\
	boost::assign::fun BOOST_PP_ENUM(N,BOOST_ASSIGN_LIST_OF_CSV_tmp,~)		\
/**/    
#define BOOST_ASSIGN_LIST_OF_CSV_ITER(fun,N)								\
namespace boost{															\
namespace assign{															\
	template<typename T>													\
   	typename assign::detail::auto_size::result_of_copy<const T,N>::type		\
	fun(BOOST_PP_ENUM_PARAMS(N, const T& _)){								\
		return BOOST_ASSIGN_LIST_OF_CSV_rec(fun,N); 						\
    }																		\
}																			\
}																			\
/**/


// TODO BOOST_PP_REPEAT and parameterize for const-qual/Ref

#endif
