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
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Let n = BOOST_ASSIGN_CSV_SIZE and a1,...,an objects of type T and 
// Ref an alias for BOOST_ASSIGN_CSV_ref
// Usage:
// BOOST_ASSIGN_CSV(fun) creates for i=2,...,n the following overloads:
// 	fun(a1,..,.ai) 
// 	cfun(a1,..,.ai) 
// which return the same result as calling fun(a1)...(ai) and cfun(a1)...(ai),
// respectively.
//
// Requirements:
// Valid expression				Result
// fun(a1)(a2)...(an) 			auto_size::result_of<T,n,Ref>::type
// cfun(a1)(a2)...(an)			auto_size::result_of<const T,n,Ref>::type

#ifndef BOOST_ASSIGN_CSV_SIZE
#define BOOST_ASSIGN_CSV_SIZE 20
#endif

#ifndef BOOST_ASSIGN_CSV_ref
#error
#endif

#define BOOST_ASSIGN_CSV_ARG(z,n,arg) (BOOST_PP_CAT(arg,n))
#define BOOST_ASSIGN_CSV_CALL(fun,N) 										\
	boost::assign::fun BOOST_PP_ENUM(N,BOOST_ASSIGN_CSV_ARG,~)				\
/**/    


#define BOOST_ASSIGN_CSV_ITER_UNQUAL(F,T,U,N)								\
namespace boost{															\
namespace assign{															\
	template<typename T>													\
   	typename assign::detail::auto_size::result_of<							\
    	U,N,BOOST_ASSIGN_CSV_ref>::type										\
	F(BOOST_PP_ENUM_PARAMS(N, U& _)){										\
        return (boost::assign::F 											\
        	BOOST_PP_REPEAT(N,BOOST_ASSIGN_CSV_ARG,_)						\
        ).allocated(); 														\
    }																		\
}																			\
}																			\
/**/

#define BOOST_ASSIGN_CSV_ITER(fun,N)										\
	BOOST_ASSIGN_CSV_ITER_UNQUAL(fun,T,T,N)									\
	BOOST_ASSIGN_CSV_ITER_UNQUAL(BOOST_PP_CAT(c,fun),T,const T,N)			\
/**/

// overloads begin at n = 2
#define BOOST_ASSIGN_CSV_SHIFTED_ITER(z,n,F) 								\
	BOOST_ASSIGN_CSV_ITER(F,BOOST_PP_ADD(n,2))								\
/**/

#define BOOST_ASSIGN_CSV_REPEAT(fun,N) 										\
	BOOST_PP_REPEAT(BOOST_PP_DEC(N),BOOST_ASSIGN_CSV_SHIFTED_ITER,fun)		\
/**/

#define BOOST_ASSIGN_CSV(fun) 												\
	BOOST_ASSIGN_CSV_REPEAT(fun,BOOST_ASSIGN_CSV_SIZE)						\
/**/

#endif
