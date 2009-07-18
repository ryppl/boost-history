// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/result_of.hpp>

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

#define CTOR_SPECIALIZATION(COMBINATION,_)\
        BOOST_FUSION_ADAPTER_NAME(BOOST_FUSION_ADAPTER_NAME COMBINATION adapter)\
          : f(std::forward<BOOST_FUSION_ADAPTER_NAME COMBINATION>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(CTOR_SPECIALIZATION,_)

#undef CTOR_SPECIALIZATION

        template<typename OtherF>
        BOOST_FUSION_ADAPTER_NAME&
        operator=(OtherF&& other_f)
        {
            f=std::forward<OtherF>(other_f);
            return *this;
        }

#define ASSIGN_SPECIALIZATION(COMBINATION,_)\
        BOOST_FUSION_ADAPTER_NAME&\
        operator=(BOOST_FUSION_ADAPTER_NAME COMBINATION other_adapter)\
        {\
            f=std::forward<BOOST_FUSION_ADAPTER_NAME COMBINATION>(\
                    other_adapter).f;\
            return *this;\
        }

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(ASSIGN_SPECIALIZATION,_)

#undef ASSIGN_SPECIALIZATION

#ifdef BOOST_FUSION_TYPED
#   define CALL_OPERATOR(COMBINATION)\
        template<typename... Args>\
        typename support::result_of<\
            F(typename result_of::as_vector<TransformSeq&&>::type)\
        >::type\
        operator()(Args&&... args) COMBINATION\
        {\
            return f(fusion::as_vector(\
                TransformSeq(std::forward<Args>(args)...)));\
        }
#else
#   define CALL_OPERATOR(COMBINATION)\
        template<typename... Args>\
        typename support::result_of<\
            F(typename result_of::vector_tie<Args&&...>::type)\
        >::type\
        operator()(Args&&... args) COMBINATION\
        {\
            return f(vector_tie(std::forward<Args>(args)...));\
        }
#endif

        CALL_OPERATOR(BOOST_PP_EMPTY());
        CALL_OPERATOR(const);
        CALL_OPERATOR(volatile);
        CALL_OPERATOR(const volatile);
#
#undef CALL_OPERATOR
#undef OUTER_GEENRATOR

        F f;
    };
}}

#undef BOOST_FUSION_ADAPTER_NAME

