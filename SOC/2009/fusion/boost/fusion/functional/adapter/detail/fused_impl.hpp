/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/template.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/type_traits/add_const.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/call_traits.hpp>
#endif

namespace boost { namespace fusion
{
#ifdef BOOST_MSVC
#   pragma warning(push)
    //'class' : multiple copy constructors specified
#   pragma warning(disable:4521)
    //'class' : multiple assignment operators specified
#   pragma warning(disable:4522)
#endif

    template<typename F>
    struct BOOST_FUSION_ADAPTER_NAME
    {
        template<typename Sig>
        struct result;

        template<class Self, class Seq>
        struct result<Self(Seq)>
          : result_of::BOOST_FUSION_INVOCATION_METHOD<
                typename detail::forward_as_lref<Self,F>::type
              , Seq
            >
        {};

        BOOST_FUSION_ADAPTER_NAME()
          : f()
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        BOOST_FUSION_ADAPTER_NAME(typename call_traits<F>::param_type f)
          : f(f)
        {}
#else
        template<typename... Args>
        BOOST_FUSION_ADAPTER_NAME(BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : f(BOOST_FUSION_FORWARD(Args,args)...)
        {}

#   ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
        template<typename... Args>
        BOOST_FUSION_ADAPTER_NAME(Args&... args)
          : f(args...)
        {}
#   endif
#endif

#define BOOST_FUSION_FUSED_CTOR(MODIFIER,_)\
        BOOST_FUSION_ADAPTER_NAME(BOOST_FUSION_ADAPTER_NAME MODIFIER adapter)\
          : f(static_cast<BOOST_FUSION_ADAPTER_NAME MODIFIER>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_FUSED_CTOR,_)

#undef BOOST_FUSION_FUSED_CTOR

        template<typename OtherF>
        BOOST_FUSION_ADAPTER_NAME&
        operator=(BOOST_FUSION_RREF_ELSE_OBJ(OtherF) other_f)
        {
            f=BOOST_FUSION_FORWARD(OtherF, other_f);
            return *this;
        }

#define BOOST_FUSION_ASSIGN_SPECIALIZATION(MODIFIER,_)\
        BOOST_FUSION_ADAPTER_NAME&\
        operator=(BOOST_FUSION_ADAPTER_NAME MODIFIER\
            other_adapter)\
        {\
            f=static_cast<BOOST_FUSION_ADAPTER_NAME MODIFIER>(other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_ASSIGN_SPECIALIZATION,_)

#undef BOOST_FUSION_ASSIGN_SPECIALIZATION

#define BOOST_FUSION_CALL_OPERATOR(F, SEQ_MODIFIER, MODIFIER)\
        template<typename Seq>\
        typename result_of::BOOST_FUSION_INVOCATION_METHOD<\
            F\
          , Seq SEQ_MODIFIER\
        >::type\
        operator()(Seq SEQ_MODIFIER seq) MODIFIER\
        {\
            {\
                BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))\
            }\
            {\
                BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))\
            }\
            \
            return fusion::BOOST_FUSION_INVOCATION_METHOD<F>(\
                static_cast<F>(f),\
                static_cast<Seq SEQ_MODIFIER>(seq));\
        }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
        BOOST_FUSION_CALL_OPERATOR(F,&,BOOST_PP_EMPTY())
        BOOST_FUSION_CALL_OPERATOR(F,const&,BOOST_PP_EMPTY())
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,&,const)
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const&,const)
#else
        BOOST_FUSION_CALL_OPERATOR(F,&&,BOOST_PP_EMPTY())
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,&&,const)
#endif

#undef BOOST_FUSION_CALL_OPERATOR

        F f;
    };

#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif
}}

