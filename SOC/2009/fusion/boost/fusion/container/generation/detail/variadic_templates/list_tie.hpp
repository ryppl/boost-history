// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_DETAIL_LIST_TIE
#define BOOST_FUSION_CONTAINER_DETAIL_LIST_TIE

#include <boost/fusion/container/list/list.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... Types>
        struct list_tie
        {
            typedef
                list<typename detail::as_fusion_element_lref<Types>::type...>
            type;
        };
    }

    template <typename... Types>
    inline typename
        result_of::list_tie<BOOST_FUSION_R_ELSE_LREF(Types)...>::type
    list_tie(BOOST_FUSION_R_ELSE_LREF(Types)... types)
    {
        return typename
            result_of::list_tie<BOOST_FUSION_R_ELSE_LREF(Types)...>::type(
                BOOST_FUSION_FORWARD(Types, types)...);
    }

}}

#endif


