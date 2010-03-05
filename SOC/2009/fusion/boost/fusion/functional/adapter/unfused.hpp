/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP

#include <boost/config.hpp>
#include <boost/fusion/container/generation/vector_tie.hpp>
#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/support/internal/template.hpp>
#   include <boost/preprocessor/iterate.hpp>
#endif
#include <boost/preprocessor/empty.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost { namespace fusion
{
    template<
        typename F
        //TODO remove?
      , bool=true
    >
    struct unfused
    {
        template<typename Sig>
        struct result;

#ifndef BOOST_NO_VARIADIC_TEMPLATES
        template<typename Self, typename... Args>
        struct result<Self(Args...)>
          : boost::result_of<
              typename detail::get_func_base<
                  typename detail::forward_as<Self,F>::type
               >::type(BOOST_FUSION_R_ELSE_LREF(
                   typename result_of::vector_tie<Args...>::type))
          >
        {};
#endif

        unfused()
          : f()
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        unfused(typename call_traits<F>::param_type f)
          : f(f)
        {}
#else
        template<typename... Args>
        unfused(BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : f(BOOST_FUSION_FORWARD(Args,args)...)
        {}

#   ifdef BOOST_NO_RVALUE_REFERENCES
        template<typename... Args>
        unfused(Args&... args)
          : f(args...)
        {}
#   endif
#endif

#define BOOST_FUSION_CTOR_SPECIALIZATION(MODIFIER,_)\
        unfused(unfused MODIFIER adapter)\
          : f(static_cast<unfused MODIFIER>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CTOR_SPECIALIZATION,_)

#undef BOOST_FUSION_CTOR_SPECIALIZATION

        template<typename OtherF>
        unfused&
        operator=(BOOST_FUSION_RREF_ELSE_OBJ(OtherF) other_f)
        {
            f=BOOST_FUSION_FORWARD(OtherF,other_f);
            return *this;
        }

#define BOOST_FUSION_ASSIGN_SPECIALIZATION(MODIFIER,_)\
        unfused&\
        operator=(unfused MODIFIER other_adapter)\
        {\
            f=static_cast<unfused MODIFIER>(other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_ASSIGN_SPECIALIZATION,_)

#undef BOOST_FUSION_ASSIGN_SPECIALIZATION

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_PP_FILENAME_1 <boost/fusion/functional/adapter/detail/pp/unfused_invocation_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_UNFUSED_MAX_ARITY)
#   include BOOST_PP_ITERATE()
#else
#   define BOOST_FUSION_CALL_OPERATOR(F,MODIFIER)\
        template<typename... Args>\
        typename boost::result_of<\
            typename detail::get_func_base<F>::type(\
                BOOST_FUSION_R_ELSE_LREF(typename result_of::vector_tie<\
                    BOOST_FUSION_R_ELSE_LREF(Args)...\
                >::type))\
        >::type\
        operator()(BOOST_FUSION_R_ELSE_LREF(Args)... args) MODIFIER\
        {\
            return f(fusion::vector_tie(BOOST_FUSION_FORWARD(Args,args)...));\
        }

        BOOST_FUSION_CALL_OPERATOR(F,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const);

#   undef BOOST_FUSION_CALL_OPERATOR
#endif

        F f;
    };
}}

#endif
