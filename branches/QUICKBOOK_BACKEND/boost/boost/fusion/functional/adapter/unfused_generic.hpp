/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_GENERIC_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/fusion/sequence/container/vector/vector.hpp>

#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/functional/adapter/detail/access.hpp>
#include <boost/fusion/functional/adapter/detail/nullary_call_base.hpp>

namespace boost { namespace fusion
{
    template <class Function> class unfused_generic;

    //----- ---- --- -- - -  -   -

    struct void_;

    template <class Function> class unfused_generic
        : public detail::nullary_call_base<unfused_generic<Function>, Function>
    {
        Function fnc_transformed;

        template <class D, class F, bool E>
        friend struct detail::nullary_call_base;

        typedef detail::nullary_call_base<
            fusion::unfused_generic<Function>, Function > base;

        typedef typename boost::remove_reference<Function>::type function;
        typedef typename detail::call_param<Function>::type func_const_fwd_t;

      public:

        inline explicit unfused_generic(func_const_fwd_t f = Function())
            : fnc_transformed(f)
        { }

        using base::operator();

        template <
            BOOST_PP_ENUM_BINARY_PARAMS(BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY,
                typename T, = fusion::void_ BOOST_PP_INTERCEPT),
                class _ = fusion::void_
            >
        struct result;

        template <class _>
        struct result<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY,
                fusion::void_ BOOST_PP_INTERCEPT),_>
            : base::r0
        { };

        #define BOOST_FUSION_CODE(tpl_params,arg_types,params,args)             \
        template <tpl_params>                                                  \
        inline typename function::template result<                             \
            BOOST_PP_CAT(fusion::vector,N)<arg_types> >::type                  \
        operator()(params) const                                               \
        {                                                                      \
            BOOST_PP_CAT(fusion::vector,N)<arg_types> arg(args);               \
            return this->fnc_transformed(arg);                                 \
        }                                                                      \
        template <tpl_params>                                                  \
        inline typename function::template result<                             \
            BOOST_PP_CAT(fusion::vector,N)<arg_types> >::type                  \
        operator()(params)                                                     \
        {                                                                      \
            BOOST_PP_CAT(fusion::vector,N)<arg_types> arg(args);               \
            return this->fnc_transformed(arg);                                 \
        }

        #define  BOOST_PP_INDIRECT_SELF                                        \
            <boost/fusion/functional/adapter/unfused_generic.hpp>
        #define  BOOST_PP_FILENAME_1                                           \
            <boost/fusion/functional/adapter/detail/pow2_explode.hpp>
        #define  BOOST_PP_ITERATION_LIMITS                                     \
            (1,BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY)
        #define  N BOOST_PP_ITERATION_1
        #include BOOST_PP_ITERATE()
        #undef   N
        #undef BOOST_FUSION_CODE
    };
}}

#define  BOOST_FUSION_CLASS_TPL_PARAMS class F
#define  BOOST_FUSION_CLASS_TPL_SPEC fusion::unfused_generic<F>
#define  BOOST_FUSION_FUNC_OBJ_ARITY BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY
#include <boost/fusion/functional/adapter/detail/gen_result_of_spec.hpp>

#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_GENERIC_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/fusion/functional/adapter/detail/pt_def.hpp>

#if BOOST_PP_SLOT_1() == 0 
        template <BOOST_PP_ENUM_PARAMS(N,typename T), class _>
        struct result
#if N < BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY
            < BOOST_PP_ENUM_PARAMS(N,T),
            BOOST_PP_ENUM_PARAMS(
                BOOST_PP_SUB(BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY,N),
                fusion::void_ BOOST_PP_INTERCEPT), _ >
#endif
            : function::template result< BOOST_PP_CAT(fusion::vector,N)<
                BOOST_PP_ENUM_BINARY_PARAMS(N,typename detail::gref<T,>::type
                    BOOST_PP_INTERCEPT) > >
        { };
#endif

#if BOOST_WORKAROUND(BOOST_MSVC,BOOST_TESTED_AT(1400)) 
        template <BOOST_PP_ENUM_PARAMS(N,typename T)>
        inline typename function::template result<
            BOOST_PP_CAT(fusion::vector,N)<BOOST_PP_ENUM_PARAMS(N,PT)> >::type
        operator()(BOOST_PP_ENUM_BINARY_PARAMS(N,PT,a)) const
        {
            BOOST_PP_CAT(fusion::vector,N)<BOOST_PP_ENUM_PARAMS(N,PT)>
                arg(BOOST_PP_ENUM_PARAMS(N,a));
            return this->fnc_transformed(arg);
        }
        template <BOOST_PP_ENUM_PARAMS(N,typename T)>
        inline typename function::template result<
            BOOST_PP_CAT(fusion::vector,N)<BOOST_PP_ENUM_PARAMS(N,PT)> >::type
        operator()(BOOST_PP_ENUM_BINARY_PARAMS(N,PT,a))
        {
            BOOST_PP_CAT(fusion::vector,N)<BOOST_PP_ENUM_PARAMS(N,PT)>
                arg(BOOST_PP_ENUM_PARAMS(N,a));
            return this->fnc_transformed(arg);
        }
#else
        BOOST_FUSION_CODE(BOOST_PP_ENUM_PARAMS(N,typename T),
            BOOST_PP_ENUM_PARAMS(N,PT), BOOST_PP_ENUM_BINARY_PARAMS(N,PT,a),
            BOOST_PP_ENUM_PARAMS(N,a) )
        // ...generates uglier code but is faster - it caches ENUM_*
#endif

#include <boost/fusion/functional/adapter/detail/pt_undef.hpp>

#endif // defined(BOOST_PP_IS_ITERATING)
#endif

