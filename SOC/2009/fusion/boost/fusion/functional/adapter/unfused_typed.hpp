/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES)
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#   ifdef BOOST_NO_RVALUE_REFERENCES
#       include <boost/fusion/sequence/intrinsic/value_at.hpp>
#   endif
#endif
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/support/internal/template.hpp>
#   include <boost/preprocessor/iterate.hpp>
#endif
#include <boost/preprocessor/empty.hpp>
#include <boost/type_traits/add_const.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/call_traits.hpp>
#endif

//TODO cschmidt: tagged variadic, non-rvalue implementation

namespace boost { namespace fusion
{
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES)
    template<typename F, typename TransformSeq>
    struct unfused_typed;

    namespace detail
    {
        template<int SeqSize, typename F, typename TransformSeq>
        struct unfused_typed_impl;

#   define BOOST_PP_FILENAME_1 <boost/fusion/functional/adapter/detail/pp/unfused_typed_impl_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY)
#   include BOOST_PP_ITERATE()
    }
#endif

    template<typename F, typename TransformSeq>
    struct unfused_typed
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES)
      : detail::unfused_typed_impl<
            result_of::size<TransformSeq>::value
          , F
          , TransformSeq
        >
#endif
    {
#if !defined(BOOST_NO_VARIADIC_TEMPLATES) && !defined(BOOST_NO_RVALUE_REFERENCES)
        typedef typename
            result_of::as_vector<BOOST_FUSION_R_ELSE_CLREF(TransformSeq)>::type
        transformed_args;

        template<typename Sig>
        struct result;

        template<typename Self, typename... Args>
        struct result<Self(Args...)>
          : boost::result_of<
                typename detail::get_func_base<
                    typename detail::forward_as<Self,F>::type
                >::type(transformed_args&&)
            >
        {};
#endif

        unfused_typed()
          : f()
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        unfused_typed(typename call_traits<F>::param_type f)
          : f(f)
        {}
#else
        template<typename... Args>
        unfused_typed(BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : f(BOOST_FUSION_FORWARD(Args,args)...)
        {}

#   ifdef BOOST_NO_RVALUE_REFERENCES
        template<typename... Args>
        unfused_typed(Args&... args)
          : f(args...)
        {}
#   endif
#endif

#define BOOST_FUSION_CTOR_SPECIALIZATION(MODIFIER,_)\
        unfused_typed(unfused_typed MODIFIER adapter)\
          : f(static_cast<unfused_typed MODIFIER>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CTOR_SPECIALIZATION,_)

#undef BOOST_FUSION_CTOR_SPECIALIZATION

        template<typename OtherF>
        unfused_typed&
        operator=(BOOST_FUSION_RREF_ELSE_OBJ(OtherF) other_f)
        {
            f=BOOST_FUSION_FORWARD(OtherF,other_f);
            return *this;
        }

#define BOOST_FUSION_ASSIGN_SPECIALIZATION(MODIFIER,_)\
        unfused_typed&\
        operator=(unfused_typed MODIFIER other_adapter)\
        {\
            f=static_cast<unfused_typed MODIFIER>(other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_ASSIGN_SPECIALIZATION,_)

#undef BOOST_FUSION_ASSIGN_SPECIALIZATION

#if !defined(BOOST_NO_VARIADIC_TEMPLATES) && !defined(BOOST_NO_RVALUE_REFERENCES)
#   define BOOST_FUSION_CALL_OPERATOR(F,MODIFIER)\
        template<typename... Args>\
        typename boost::result_of<\
            typename detail::get_func_base<F>::type(transformed_args&&)\
        >::type\
        operator()(Args&&... args) MODIFIER\
        {\
            return f(fusion::as_vector(\
                TransformSeq(std::forward<Args>(args)...)));\
        }

        BOOST_FUSION_CALL_OPERATOR(F,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const);

#   undef BOOST_FUSION_CALL_OPERATOR
#endif

        F f;
    };
}}

#undef BOOST_FUSION_ADAPTER_NAME

#endif
