//////////////////////////////////////////////////////////////////////////////
// assign::detail::csv.hpp                                                  //
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
#include <boost/assign/auto_size/array/wrapper.hpp>

// Whereas adjacent unary function calls is the usual way to create a collec-
// tion in Boost.Assign, this macro provides, as an alternative, functions that 
// are overloaded on the number of arguments.
//
// Let n = BOOST_ASSIGN_CSV_SIZE and a1,...,an, objects of type T, Ref an alias 
// for BOOST_ASSIGN_CSV_ref, and w<U,N> and alias for
// array_wrapper<U,N,BOOST_ASSIGN_CSV_ref>. 
//
// Usage:
// BOOST_ASSIGN_CSV(fun) creates for i=2,...,n the following overloads:
// 	fun_csv(a1,..,.ai) 
// 	cfun_csv(a1,..,.ai) 
// which return instances of w<T,i> and w<const T,i>, respectively.

#ifndef BOOST_ASSIGN_CSV_SIZE
#define BOOST_ASSIGN_CSV_SIZE 20
#endif

#ifndef BOOST_ASSIGN_CSV_ref
#error
#endif

#define BOOST_ASSIGN_CSV_ARG(z,n,arg) (BOOST_PP_CAT(arg,n))

#define BOOST_ASSIGN_CSV_ITER_UNQUAL(F,T,U,N)                               \
namespace boost{                                                            \
namespace assign{                                                           \
    template<typename T>                                                    \
    boost::assign::detail::auto_size::array_wrapper<                        \
        U,N,BOOST_ASSIGN_CSV_ref                                            \
    >                                                                       \
    F(BOOST_PP_ENUM_PARAMS(N, U& _))                                        \
    {                                                                       \
        typedef boost::assign::detail::auto_size::array_wrapper<            \
            U,N,BOOST_ASSIGN_CSV_ref> wrapper_;                             \
        wrapper_ wrapper;                                                   \
        wrapper.initialize(                                                 \
            boost::assign::detail::auto_size::make_first_expr_no_policy<    \
                BOOST_ASSIGN_CSV_ref,T                                      \
            > BOOST_PP_REPEAT(N,BOOST_ASSIGN_CSV_ARG,_)                     \
        );                                                                  \
        return wrapper;                                                     \
    }                                                                       \
}                                                                           \
}                                                                           \
/**/

// Note : ming

#define BOOST_ASSIGN_CSV_ITER(F,N)                                          \
    BOOST_ASSIGN_CSV_ITER_UNQUAL(F,T,T,N)                                   \
    BOOST_ASSIGN_CSV_ITER_UNQUAL(BOOST_PP_CAT(c,F),T,const T,N)             \
/**/

// overloads begin at n = 1
#define BOOST_ASSIGN_CSV_SHIFTED_ITER(z,n,F)                                \
    BOOST_ASSIGN_CSV_ITER(F,BOOST_PP_ADD(n,1))                              \
/**/

#define BOOST_ASSIGN_CSV_REPEAT(fun,N)                                      \
    BOOST_PP_REPEAT(N,BOOST_ASSIGN_CSV_SHIFTED_ITER,fun)                    \
/**/

#define BOOST_ASSIGN_CSV(fun)                                               \
    BOOST_ASSIGN_CSV_REPEAT(fun,BOOST_ASSIGN_CSV_SIZE)                      \
/**/

#endif
