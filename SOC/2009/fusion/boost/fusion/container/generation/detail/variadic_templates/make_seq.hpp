// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... Types>
        struct BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME)
        {
            typedef
                BOOST_FUSION_SEQ_NAME<
                    typename detail::as_fusion_element<Types>::type...
                >
            type;
        };
    }

    template <typename... Types>
    inline typename
        result_of::BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME)<
            BOOST_FUSION_R_ELSE_CLREF(Types)...
        >::type
    BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME)(
        BOOST_FUSION_R_ELSE_CLREF(Types)... types)
    {
        return typename
            result_of::BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME)<
                BOOST_FUSION_R_ELSE_CLREF(Types)...
            >::type(BOOST_FUSION_FORWARD(Types, types)...);
    }
}}

