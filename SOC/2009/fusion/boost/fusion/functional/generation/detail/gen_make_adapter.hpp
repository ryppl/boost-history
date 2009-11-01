/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/deduce.hpp>

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
                    typename traits::deduce<F>::type
                >
            type;
        };
    }

    template<typename F>
    inline typename
        result_of::BOOST_FUSION_FUNC_NAME<BOOST_FUSION_RREF_ELSE_OBJ(F)>::type
    BOOST_FUSION_FUNC_NAME(BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return typename
            result_of::BOOST_FUSION_FUNC_NAME<BOOST_FUSION_RREF_ELSE_OBJ(F)>::
                type(BOOST_FUSION_FORWARD(F,f));
    }
}}

#undef BOOST_FUSION_CLASS_TPL_NAME

