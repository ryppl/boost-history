// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_DETAIL_MAKE_VECTOR
#define BOOST_FUSION_CONTAINER_DETAIL_MAKE_VECTOR

#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename... Types>
        struct make_vector
        {
            typedef
                vector<typename detail::as_fusion_element<Types>::type...>
            type;
        };
    }

    //TODO cschmidt: ?!
    template <typename... Types>
    inline typename
        result_of::make_vector<BOOST_FUSION_R_ELSE_CLREF(Types)...>::type
    make_vector(BOOST_FUSION_R_ELSE_CLREF(Types)... types)
    {
        return typename
            result_of::make_vector<BOOST_FUSION_R_ELSE_CLREF(Types)...>::type(
                    BOOST_FUSION_FORWARD(Types, types)...);
    }

}}

#endif

