// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_TUPLE_DETAIL_VARIADIC_TEMPLATES_TUPLE_TIE_HPP
#define BOOST_FUSION_TUPLE_DETAIL_VARIADIC_TEMPLATES_TUPLE_TIE_HPP

#include <boost/fusion/tuple/tuple.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    template <typename... Types>
    inline tuple<typename detail::as_fusion_element_ref<Types>::type...>
    tie(BOOST_FUSION_R_ELSE_LREF(Types)... types)
    {
        return tuple<typename detail::as_fusion_element_ref<Types>::type...>(
                   BOOST_FUSION_FORWARD(Types, types)...);
    }

}}

#endif
