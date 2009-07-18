/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

// No include guard - this file is included multiple times intentionally.

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/preprocessor/cat.hpp>

#define BOOST_FUSION_FUNC_NAME BOOST_PP_CAT(make_,BOOST_FUSION_CLASS_TPL_NAME)

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename F>
        struct BOOST_FUSION_FUNC_NAME
        {
            typedef
                BOOST_FUSION_CLASS_TPL_NAME<
                    typename fusion::detail::as_fusion_element<F>::type
                >
            type;
        };
    }

    template <typename F>
    inline typename
        result_of::BOOST_FUSION_FUNC_NAME<BOOST_FUSION_R_ELSE_LREF(F)>::type
    BOOST_FUSION_FUNC_NAME(BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return typename
            result_of::BOOST_FUSION_FUNC_NAME<BOOST_FUSION_R_ELSE_LREF(F)>::
                type(BOOST_FUSION_FORWARD(F,f));
    }
}}

#undef BOOST_FUSION_CLASS_TPL_NAME
#undef BOOST_FUSION_FUNC_NAME

