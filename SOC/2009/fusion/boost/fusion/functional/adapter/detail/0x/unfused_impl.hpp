/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>

#include <boost/preprocessor/empty.hpp>

#ifdef BOOST_FUSION_TYPED
#   include <boost/fusion/container/vector/convert.hpp>

#   define BOOST_FUSION_ADAPTER_NAME unfused_typed
#else
#   include <boost/fusion/container/generation/vector_tie.hpp>

#   define BOOST_FUSION_ADAPTER_NAME unfused
#endif

namespace boost { namespace fusion
{
    template<
        typename F
#ifdef BOOST_FUSION_TYPED
      , typename TransformSeq
#else
      , bool=true
#endif
    >
    struct BOOST_FUSION_ADAPTER_NAME
    {
        BOOST_FUSION_ADAPTER_NAME()
          : f()
        {}

        template<typename... Args>
        BOOST_FUSION_ADAPTER_NAME(Args&&... args)
          : f(std::forward<Args>(args)...)
        {}

#define BOOST_FUSION_CTOR_SPECIALIZATION(MODIFIER,_)\
        BOOST_FUSION_ADAPTER_NAME(BOOST_FUSION_ADAPTER_NAME MODIFIER adapter)\
          : f(std::forward<BOOST_FUSION_ADAPTER_NAME MODIFIER>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CTOR_SPECIALIZATION,_)

#undef BOOST_FUSION_CTOR_SPECIALIZATION

        template<typename OtherF>
        BOOST_FUSION_ADAPTER_NAME&
        operator=(OtherF&& other_f)
        {
            f=std::forward<OtherF>(other_f);
            return *this;
        }

#define BOOST_FUSION_ASSIGN_SPECIALIZATION(MODIFIER,_)\
        BOOST_FUSION_ADAPTER_NAME&\
        operator=(BOOST_FUSION_ADAPTER_NAME MODIFIER other_adapter)\
        {\
            f=std::forward<BOOST_FUSION_ADAPTER_NAME MODIFIER>(\
                    other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_ASSIGN_SPECIALIZATION,_)

#undef BOOST_FUSION_ASSIGN_SPECIALIZATION

#ifdef BOOST_FUSION_TYPED
#   define BOOST_FUSION_CALL_OPERATOR(MODIFIER)\
        template<typename... Args>\
        typename boost::result_of<\
            typename detail::get_func_base<\
                typename detail::forward_as<int MODIFIER, F>::type\
            >::type(typename result_of::as_vector<TransformSeq&&>::type&&)\
        >::type\
        operator()(Args&&... args) MODIFIER\
        {\
            return f(fusion::as_vector(\
                TransformSeq(std::forward<Args>(args)...)));\
        }
#else
#   define BOOST_FUSION_CALL_OPERATOR(MODIFIER)\
        template<typename... Args>\
        typename boost::result_of<\
            typename detail::get_func_base<\
                typename detail::forward_as<int MODIFIER, F>::type\
            >::type(typename result_of::vector_tie<Args&&...>::type&&)\
        >::type\
        operator()(Args&&... args) MODIFIER\
        {\
            return f(vector_tie(std::forward<Args>(args)...));\
        }
#endif

        BOOST_FUSION_CALL_OPERATOR(BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(const);

#undef BOOST_FUSION_CALL_OPERATOR

        F f;
    };
}}

#undef BOOST_FUSION_ADAPTER_NAME


