// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/result_of.hpp>

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

#define CTOR_SPECIALIZATION(COMBINATION,_)\
        BOOST_FUSION_ADAPTER_NAME(BOOST_FUSION_ADAPTER_NAME COMBINATION adapter)\
          : f(std::forward<BOOST_FUSION_ADAPTER_NAME COMBINATION>(adapter).f)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(CTOR_SPECIALIZATION,_)

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

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(ASSIGN_SPECIALIZATION,_)

#undef ASSIGN_SPECIALIZATION

#define CALL_OPERATOR(COMBINATION)\
        template<typename Seq>\
        typename result_of::invoke<\
            typename detail::forward_as<int COMBINATION, F>::type, Seq&&\
        >::type\
        operator()(Seq&& seq) COMBINATION\
        {\
            return\
                invoke<typename detail::forward_as<int COMBINATION, F>::type>(\
                        f,std::forward<Seq>(seq));\
        }

        CALL_OPERATOR(BOOST_PP_EMPTY());
        CALL_OPERATOR(const);

#undef CALL_OPERATOR

        F f;
    };
}}

#undef BOOST_FUSION_ADAPTER_NAME

