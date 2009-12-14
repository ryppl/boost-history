/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion
{
    template<typename F>
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

#define BOOST_FUSION_CALL_OPERATOR(MODIFIER)\
        template<typename Seq>\
        typename result_of::invoke<\
            typename detail::forward_as<int MODIFIER, F>::type, Seq&&\
        >::type\
        operator()(Seq&& seq) MODIFIER\
        {\
            /*BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));*/\
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));\
            \
            return invoke(\
                static_cast<\
                    typename detail::forward_as<int MODIFIER, F>::type\
                >(f),\
                std::forward<Seq>(seq));\
        }

        BOOST_FUSION_CALL_OPERATOR(BOOST_PP_EMPTY());
        BOOST_FUSION_CALL_OPERATOR(const);

#undef BOOST_FUSION_CALL_OPERATOR

        F f;
    };
}}

