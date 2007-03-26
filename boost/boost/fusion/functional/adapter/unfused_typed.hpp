/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/type_traits/remove_reference.hpp>

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/fusion/sequence/conversion/as_vector.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>

#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/functional/adapter/detail/has_type.hpp>
#include <boost/fusion/functional/adapter/detail/nullary_call_base.hpp>


namespace boost { namespace fusion
{

    template <class Function, class Sequence> class unfused_typed;

    //----- ---- --- -- - -  -   -

    namespace detail
    {
        template <class Derived, class Function, class Sequence,
            long Arity, bool EnableCallOp>
        struct unfused_typed_impl;

        template <class Derived, class Function, class Sequence,
            class NextSeq = typename result_of::pop_back<Sequence>::type >
        struct unfused_typed_next_base 
        {
            // type of the next base class
            typedef unfused_typed_impl
                < Derived, Function, NextSeq, result_of::size<NextSeq>::value,
                  has_type< typename Function::template result< 
                      typename result_of::as_vector<NextSeq>::type > >::value
                >
            type; 
        };

        template <class Derived, class Function, class Sequence, long Arity>
        struct unfused_typed_impl<Derived,Function,Sequence,Arity,false>
            : unfused_typed_next_base<Derived,Function,Sequence>::type
        { };

        template <class Derived, class Function, class Sequence>
        struct unfused_typed_impl<Derived,Function,Sequence,0,false>
            : nullary_call_base<Derived,Function,false>
        { };

        template <class Derived, class Function, class Sequence>
        struct unfused_typed_impl<Derived,Function,Sequence,0,true>
            : nullary_call_base<Derived,Function,true>
        { };

    }

    template <class Function, class Sequence>
    class unfused_typed
        : public detail::unfused_typed_next_base
          < unfused_typed<Function,Sequence>,
            typename remove_reference<Function>::type, Sequence, Sequence
          >::type
    {
        Function fnc_transformed;

        template <class D, class F, class S, long A, bool EO>
        friend struct detail::unfused_typed_impl;

        template <class D, class F, bool E>
        friend struct detail::nullary_call_base;

        typedef typename boost::remove_reference<Function>::type function;
        typedef typename detail::call_param<Function>::type func_const_fwd_t;

        typedef typename detail::unfused_typed_next_base<unfused_typed<
            Function,Sequence>,function,Sequence,Sequence>::type base;

    public:

        inline explicit unfused_typed(func_const_fwd_t f = Function())
            : fnc_transformed(f)
        { }

        template <
            BOOST_PP_ENUM_BINARY_PARAMS(BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY,
                typename T, = fusion::void_ BOOST_PP_INTERCEPT),
                class _ = fusion::void_
            >
        struct result;

        template <typename _>
        struct result<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY,
                fusion::void_ BOOST_PP_INTERCEPT),_>
            : base::r0
        { };
    }; 

    #define  BOOST_PP_FILENAME_1 <boost/fusion/functional/adapter/unfused_typed.hpp>
    #define  BOOST_PP_ITERATION_LIMITS (1,BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY)
    #include BOOST_PP_ITERATE() 

}}

#define  BOOST_FUSION_CLASS_TPL_PARAMS class F, class S
#define  BOOST_FUSION_CLASS_TPL_SPEC fusion::unfused_typed<F,S>
#define  BOOST_FUSION_FUNC_OBJ_ARITY BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY
#include <boost/fusion/functional/adapter/detail/gen_result_of_spec.hpp>

#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#define N BOOST_PP_ITERATION()

    namespace detail
    {

        template <class Derived, class Function, class Sequence>
        struct unfused_typed_impl<Derived,Function,Sequence,N,true>
            : unfused_typed_next_base<Derived,Function,Sequence>::type
        {
        private:
            typedef typename unfused_typed_next_base<
                Derived,Function,Sequence>::type base;

            // Notes: 
            // - conversion to fusion::vector might not be perfect (there is 
            //   currently no "inrinsic converting ctor" that would allow us
            //   to let the user choose the sequence implementation)
            // - value_at_c (instead of iteration) is OK because of conversion 
            //   to fusion::vector - we would need iteration for arbitrary 
            //   sequences
            typedef typename result_of::as_vector<Sequence>::type arg_vector_t;
        public:

            using base::operator();

            typedef typename Function::
                template result<arg_vector_t> BOOST_PP_CAT(r,N);

#define M(z,i,s)                                                                \
    typename call_param<typename result_of::value_at_c<s,i>::type>::type a##i

            inline typename Function::template result<arg_vector_t>::type 
            operator()(BOOST_PP_ENUM(N,M,arg_vector_t)) const
            {
                arg_vector_t arg(BOOST_PP_ENUM_PARAMS(N,a));
                return static_cast<Derived const *>(this)->fnc_transformed(arg);
            }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
            inline typename Function::template result<arg_vector_t>::type 
            operator()(BOOST_PP_ENUM(N,M,arg_vector_t)) 
            {
                arg_vector_t arg(BOOST_PP_ENUM_PARAMS(N,a));
                return static_cast<Derived *>(this)->fnc_transformed(arg);
            }
#endif

#undef M
        };

    } // namespace detail

    template <class Function, class Sequence> 
        template <BOOST_PP_ENUM_PARAMS(N,typename T), class _>
    struct unfused_typed<Function,Sequence>::result
#if N < BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY
            < BOOST_PP_ENUM_PARAMS(N,T),
            BOOST_PP_ENUM_PARAMS(
                BOOST_PP_SUB(BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY,N),
                fusion::void_ BOOST_PP_INTERCEPT), _ >
#endif
        : BOOST_PP_CAT(base::r,N)
    { };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
#endif

