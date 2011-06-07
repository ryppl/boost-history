//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_CRTP_UNARY_AND_UP_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_CRTP_UNARY_AND_UP_ER_2011_HPP

#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#ifndef BOOST_ASSIGN_V2_ENABLE_CPP0X
#error
#endif

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X

// The crtp defined below makes it easy to overload on the number of
// arguments, while allowing within a certain limit, any mixture of
// lvalue and rvalues. Specifically,
// let
//     m = BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY
//     n = BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY
//
// Given a metafunction class, F, and a derived class, D, that defines
//  template<typename T0, ..., typename Tk>
//  typename apply1<F, mpl::vector<T0,...,Tk> >::type
//    impl(T0& , ..., Tk&)const;
// for k = 0, ..., n-1, crtp<F,D> defines, under C++03, a set of overloaded
// operator()'s, each forwarding to impl(), for each combination of lvalues and
// const arguments for k = 0, ..., m-1, and either only lvalues or only const
// for k = m, ..., n-1.

#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/seq.hpp>

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
#include <boost/assign/v2/support/config/limit_functor_arity.hpp>
#include <boost/assign/v2/support/config/limit_functor_const_non_const_arity.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

    template<typename D, typename F>
    class crtp_unary_and_up
    {
        protected:

        D& derived(){ return static_cast<D&>(*this); }
        D const& derived()const{ return static_cast<D const&>(*this); }

        public:

// Non-const/const overloads :

#define BOOST_ASSIGN_V2_MACRO1(r, SeqU) \
    template<BOOST_ASSIGN_V2_TPL_PARAMETER_LIST(SeqU)>\
    typename ::boost::mpl::apply1<\
        F,\
        ::boost::mpl::vector<BOOST_ASSIGN_V2_TPL_ARG_LIST(SeqU)>\
    >::type\
    operator()( BOOST_ASSIGN_V2_PARAMETER_LIST(SeqU, 0) )const{\
        return this->derived().impl(\
            BOOST_ASSIGN_V2_ARG_LIST(SeqU, 0)\
        );\
    }\
/**/

#define BOOST_ASSIGN_V2_MACRO2(z, n, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT(\
    BOOST_ASSIGN_V2_MACRO1, \
    BOOST_PP_SEQ_FIRST_N(\
        BOOST_PP_INC(n),\
        BOOST_ASSIGN_V2_SEQ_TPL_BINARY_ARG_LIST\
    )\
) \
/**/

BOOST_PP_REPEAT(
    BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY,
    BOOST_ASSIGN_V2_MACRO2,
    ~
)

#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

// Non-const only / const-only overloads :

#define BOOST_ASSIGN_V2_MACRO(z, N, data) \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    typename ::boost::mpl::apply1< \
        F, \
        BOOST_PP_CAT(::boost::mpl::vector, N)<BOOST_PP_ENUM_PARAMS(N, T)> \
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
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_CONST_NON_CONST_ARITY),
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)

#undef BOOST_ASSIGN_V2_MACRO

    };

}// functor_aux
}// v2
}// assign
}// boost

#endif // !BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_CRTP_UNARY_AND_UP_ER_2011_HPP
