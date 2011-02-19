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
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>

#include <iostream> // TODO remove

#ifndef BOOST_ASSIGN_V2_ENABLE_CPP0X
#error
#endif

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

// There should be no use for this file under CPP0x

#else

// The crtp defined below makes it easy to overload on the number of
// arguments, while allowing within a certain limit, any mixture of
// lvalue and rvalues. Specifically,
// let
// 	m = BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY
// 	n = BOOST_ASSIGN_V2_LIMIT_ARITY
//
// Given a metafunction class, F, and a derived class, D, that defines
//  template<typename T0, ..., typename Tk>
//  typename apply1<F, mpl::vector<T0,...,Tk> >::type
//	impl(T0& , ..., Tk&)const;
// for k = 0, ..., n-1, crtp<F,D> defines, under C++03, a set of overloaded
// operator()'s, each forwarding to impl(), for each combination of lvalues and
// const arguments for k = 0, ..., m-1, and either only lvalues or only const
// for k = m, ..., n-1.

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/first_n.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/config/limit_lvalue_const_arity.hpp>
#include <boost/assign/v2/detail/pp/args.hpp>
#include <boost/assign/v2/detail/pp/params.hpp>
#include <boost/assign/v2/detail/pp/seq.hpp>

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

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

// do nothing

#else

#define BOOST_ASSIGN_V2_MACRO1(r, SeqU) \
    template<BOOST_ASSIGN_V2_decl_params(SeqU)> \
    typename ::boost::mpl::apply1< \
        F, \
        ::boost::mpl::vector<BOOST_ASSIGN_V2_params(SeqU)> \
    >::type \
    operator()( BOOST_ASSIGN_V2_decl_args(SeqU) )const{ \
        return this->derived().template impl<BOOST_ASSIGN_V2_params(SeqU)>( \
            BOOST_ASSIGN_V2_args(SeqU) \
        ); \
    } \
/**/


#define BOOST_ASSIGN_V2_MACRO2(z, n, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
	BOOST_ASSIGN_V2_MACRO1, \
    BOOST_PP_SEQ_FIRST_N(BOOST_PP_INC(n), BOOST_ASSIGN_V2_SEQ)\
) \
/**/
BOOST_PP_REPEAT(
	BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY,
    BOOST_ASSIGN_V2_MACRO2,
    ~
)

#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

#define BOOST_ASSIGN_V2_MACRO(z, N, data) \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    typename ::boost::mpl::apply1< \
        F, \
        BOOST_PP_CAT(boost::mpl::vector,N)<BOOST_PP_ENUM_PARAMS(N, T)> \
    >::type \
    operator()( BOOST_PP_ENUM_BINARY_PARAMS(N, T, &_) )const{ \
        return this->derived().template impl< \
            BOOST_PP_ENUM_PARAMS(N, T) \
        >( BOOST_PP_ENUM_PARAMS(N, _) ); \
    } \
    \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    typename ::boost::mpl::apply1< \
        F, \
        ::boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N, const T)> \
    >::type \
    operator()( BOOST_PP_ENUM_BINARY_PARAMS(N, T, const &_) )const{ \
        return this->derived().template impl< \
            BOOST_PP_ENUM_PARAMS(N, const T) \
        >( BOOST_PP_ENUM_PARAMS(N, _) ); \
    } \
/**/

BOOST_PP_REPEAT_FROM_TO(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO

#endif // #if BOOST_ASSIGN_V2_ENABLE_CPP0X

	};

}// functor_aux
}// v2
}// assign
}// boost


#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CRTP_UNARY_AND_UP_ER_2010_HPP
