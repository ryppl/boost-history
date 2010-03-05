/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/add_const.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/call_traits.hpp>
#endif

namespace boost { namespace fusion
{
    template<typename F>
    struct BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)
    {
        template<typename Sig>
        struct result;

        template<class Self, class Seq>
        struct result<Self(Seq)>
          : result_of::BOOST_PP_CAT(invoke,BOOST_FUSION_ADAPTER_POSTFIX)<
                typename detail::forward_as<Self,F>::type
              , Seq
            >
        {};

        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)()
          : f()
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)(
            typename call_traits<F>::param_type f)
          : f(f)
        {}
#else
        template<typename... Args>
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)(
            BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : f(BOOST_FUSION_FORWARD(Args,args)...)
        {}

#   ifdef BOOST_NO_RVALUE_REFERENCES
        template<typename... Args>
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)(Args&... args)
          : f(args...)
        {}
#   endif
#endif

#define BOOST_FUSION_CTOR_SPECIALIZATION(MODIFIER,_)\
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)(\
            BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX) MODIFIER adapter)\
          : f(static_cast<\
                BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX) MODIFIER\
            >(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CTOR_SPECIALIZATION,_)

#undef BOOST_FUSION_CTOR_SPECIALIZATION

        template<typename OtherF>
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)&
        operator=(BOOST_FUSION_RREF_ELSE_OBJ(OtherF) other_f)
        {
            f=BOOST_FUSION_FORWARD(OtherF, other_f);
            return *this;
        }

#define BOOST_FUSION_ASSIGN_SPECIALIZATION(MODIFIER,_)\
        BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX)&\
        operator=(BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX) MODIFIER\
            other_adapter)\
        {\
            f=static_cast<\
                BOOST_PP_CAT(fused,BOOST_FUSION_ADAPTER_POSTFIX) MODIFIER\
            >(other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_ASSIGN_SPECIALIZATION,_)

#undef BOOST_FUSION_ASSIGN_SPECIALIZATION

#define BOOST_FUSION_CALL_OPERATOR(F, SEQ_MODIFIER, MODIFIER)\
        template<typename Seq>\
        typename result_of::BOOST_PP_CAT(invoke,BOOST_FUSION_ADAPTER_POSTFIX)<\
            F\
          , Seq SEQ_MODIFIER\
        >::type\
        operator()(Seq SEQ_MODIFIER seq) MODIFIER\
        {\
            /*BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));*/\
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));\
            \
            return fusion::BOOST_PP_CAT(invoke,BOOST_FUSION_ADAPTER_POSTFIX)<F>(\
                            f,\
                            static_cast<Seq SEQ_MODIFIER>(seq));\
        }

#ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_CALL_OPERATOR(F,&,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(F,const&,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,&,const);
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,const&,const);
#else
        BOOST_FUSION_CALL_OPERATOR(F,&&,BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(typename add_const<F>::type,&&,const);
#endif

#undef BOOST_FUSION_CALL_OPERATOR

        F f;
    };
}}

