// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_GENERATION_DETAIL_VARIADIC_TEMPLATES_VECTOR_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_DETAIL_VARIADIC_TEMPLATES_VECTOR_TIE_HPP

#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... Types>
        struct vector_tie
        {
            typedef
                vector<typename detail::as_fusion_element_ref<Types>::type...>
            type;
        };
    }

    template <typename... Types>
    inline typename
        result_of::vector_tie<BOOST_FUSION_R_ELSE_LREF(Types)...>::type
    vector_tie(BOOST_FUSION_R_ELSE_LREF(Types)... types)
    {
        return typename
            result_of::vector_tie<BOOST_FUSION_R_ELSE_LREF(Types)...>::type(
                BOOST_FUSION_FORWARD(Types, types)...);
    }

}}

#endif
