//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CRTP_UNARY_AND_UP_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CRTP_UNARY_AND_UP_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/detail/config/arity_bound.hpp>
//#include <boost/assign/v2/detail/config/enable_cpp_0x.hpp>

#ifdef BOOST_ASSIGN_V2_LVALUE_CONST_ARITY_BOUND
#error 
#endif 
// This constant should not be changed by the user.
#define BOOST_ASSIGN_V2_LVALUE_CONST_ARITY_BOUND 4 

// Notation:
// 	m = BOOST_ASSIGN_V2_LVALUE_CONST_ARITY_BOUND
// 	n = BOOST_ASSIGN_V2_ARITY_BOUND
//
// Usage: 
// Publicly inheriting from crtp_unary_and_up<D,F> exposes in derived class D 
// operator() for any combination of lvalues and const values for arity 
// 0 < k < m, and for m <= k < n lvalue is  preserved only if all arguments are
// lvalues.
//
// Requirements:
// For operator() with arity k to be defined, the derived class has to define, 
// 	typename apply1<F,mpl::vector<T0,...,Tk> >::type impl(T0&,...,Tk&)const;
//
// Note:
// For k = 0, operator() should be defined in the derived class, and the follo-
// wing statement added in its body:
// 	using crtp_unary_and_up<D,F>::operator();

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

	template<typename D,typename F>
	class crtp_unary_and_up
	{
		protected:

		D& derived(){ return static_cast<D&>(*this); }
		D const& derived()const{ return static_cast<D const&>(*this); }

		public:


// #ifndef BOOST_ASSIGN_V2_ENABLE_CPP_0X

#define BOOST_ASSIGN_V2_op1(U0) \
		template<typename T0> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<U0> \
        >::type \
		operator()( U0& _0)const{ \
        	return this->derived().template impl<U0>( _0 ); \
        } \
/**/
BOOST_ASSIGN_V2_op1(T0 const	)
BOOST_ASSIGN_V2_op1(T0 			)
#undef BOOST_ASSIGN_V2_op1

#define BOOST_ASSIGN_V2_op2(U0,U1) \
		template<typename T0,typename T1> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<U0, U1> \
        >::type \
		operator()( U0& _0, U1& _1)const{ \
        	return this->derived().template impl<U0, U1>( _0, _1 ); \
        } \
/**/
BOOST_ASSIGN_V2_op2(T0 const, T1 const	)
BOOST_ASSIGN_V2_op2(T0 		, T1 const	)
BOOST_ASSIGN_V2_op2(T0 const, T1 		)
BOOST_ASSIGN_V2_op2(T0		, T1 		)
#undef BOOST_ASSIGN_V2_op2

#define BOOST_ASSIGN_V2_op3(U0, U1, U2) \
		template<typename T0,typename T1,typename T2> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<U0, U1, U2> \
        >::type \
		operator()( U0& _0, U1& _1, U2& _2)const{ \
        	return this->derived().template impl<U0, U1, U2>( _0, _1, _2 ); \
        } \
/**/
BOOST_ASSIGN_V2_op3(T0 const, T1 const	, T2 const	)
BOOST_ASSIGN_V2_op3(T0 		, T1 const	, T2 const	)
BOOST_ASSIGN_V2_op3(T0 const, T1 		, T2 const	)
BOOST_ASSIGN_V2_op3(T0		, T1 		, T2 const	)
BOOST_ASSIGN_V2_op3(T0 const, T1 const	, T2 		)
BOOST_ASSIGN_V2_op3(T0 		, T1 const	, T2 		)
BOOST_ASSIGN_V2_op3(T0 const, T1 		, T2 		)
BOOST_ASSIGN_V2_op3(T0		, T1 		, T2 		)
#undef BOOST_ASSIGN_V2_op3

#define BOOST_ASSIGN_V2_op4(U0, U1, U2, U3) \
		template<typename T0,typename T1,typename T2,typename T3> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<U0, U1, U2, U3> \
        >::type \
		operator()( U0& _0, U1& _1, U2& _2, U3& _3)const{ \
        	return this->derived().template \
            	impl<U0, U1, U2, U3>( _0, _1, _2, _3 ); \
        } \
/**/
/*
BOOST_ASSIGN_V2_op4(T0 const, T1 const	, T2 const	, T3 const	)
BOOST_ASSIGN_V2_op4(T0 		, T1 const	, T2 const	, T3 const	)
BOOST_ASSIGN_V2_op4(T0 const, T1 		, T2 const	, T3 const	)
BOOST_ASSIGN_V2_op4(T0		, T1 		, T2 const	, T3 const	)
BOOST_ASSIGN_V2_op4(T0 const, T1 const	, T2 		, T3 const	)
BOOST_ASSIGN_V2_op4(T0 		, T1 const	, T2 		, T3 const	)
BOOST_ASSIGN_V2_op4(T0 const, T1 		, T2 		, T3 const	)
BOOST_ASSIGN_V2_op4(T0		, T1 		, T2 		, T3 const	)
BOOST_ASSIGN_V2_op4(T0 const, T1 const	, T2 const	, T3		)
BOOST_ASSIGN_V2_op4(T0 		, T1 const	, T2 const	, T3 		)
BOOST_ASSIGN_V2_op4(T0 const, T1 		, T2 const	, T3 		)
BOOST_ASSIGN_V2_op4(T0		, T1 		, T2 const	, T3 		)
BOOST_ASSIGN_V2_op4(T0 const, T1 const	, T2 		, T3 		)
BOOST_ASSIGN_V2_op4(T0 		, T1 const	, T2 		, T3 		)
BOOST_ASSIGN_V2_op4(T0 const, T1 		, T2 		, T3 		)
BOOST_ASSIGN_V2_op4(T0		, T1 		, T2 		, T3 		)
*/
#undef BOOST_ASSIGN_V2_op4

#define BOOST_ASSIGN_V2_op(z,N,data) \
		template<BOOST_PP_ENUM_PARAMS(N,typename T)> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,T)> \
        >::type \
		operator()( BOOST_PP_ENUM_BINARY_PARAMS(N,T,&_) )const{ \
        	return this->derived().template impl< \
            	BOOST_PP_ENUM_PARAMS(N,T) \
            >( BOOST_PP_ENUM_PARAMS(N,_) ); \
        } \
        \
		template<BOOST_PP_ENUM_PARAMS(N,typename T)> \
		typename boost::mpl::apply1< \
        	F, \
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,const T)> \
        >::type \
		operator()( BOOST_PP_ENUM_BINARY_PARAMS(N,T, const &_) )const{ \
        	return this->derived().template impl< \
            	BOOST_PP_ENUM_PARAMS(N,const T) \
            >( BOOST_PP_ENUM_PARAMS(N, _) ); \
        } \
/**/
BOOST_PP_REPEAT_FROM_TO(
	BOOST_ASSIGN_V2_LVALUE_CONST_ARITY_BOUND,
    BOOST_ASSIGN_V2_ARITY_BOUND,
    BOOST_ASSIGN_V2_op,
    ~
)
#undef BOOST_ASSIGN_V2_op

// #endif
// #ifdef BOOST_ASSIGN_V2_ENABLE_CPP_0X
// TODO
// #endif

	};

}// functor_aux
}// v2
}// assign
}// boost


#endif
