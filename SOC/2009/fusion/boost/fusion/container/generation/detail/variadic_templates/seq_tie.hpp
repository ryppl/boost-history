/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/deduce.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... Types>
        struct BOOST_FUSION_FUNCTION_NAME
        {
            typedef
                BOOST_FUSION_SEQ_NAME<
                    typename detail::deduce_ref<Types>::type...
                >
            type;
        };
    }

    template <typename... Types>
    inline typename result_of::BOOST_FUSION_FUNCTION_NAME<
        BOOST_FUSION_R_ELSE_CLREF(Types)...
    >::type
    BOOST_FUSION_FUNCTION_NAME(BOOST_FUSION_R_ELSE_CLREF(Types)... types)
    {
        return typename result_of::BOOST_FUSION_FUNCTION_NAME<
            BOOST_FUSION_R_ELSE_CLREF(Types)...
        >::type(BOOST_FUSION_FORWARD(Types, types)...);
    }
}}

