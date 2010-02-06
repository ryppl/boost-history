/*=============================================================================
    Copyright (c) 2010 Christopher Schmidts

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_SWAP_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_SWAP_HPP

#include <algorithm>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct swapper
        {
            template<class ZippedElements>
            void
            operator()(ZippedElements const& zipped_elements)const
            {
                using std::swap;
                swap(fusion::front(zipped_elements),
                    fusion::back(zipped_elements));
            }
        };
    }

    namespace result_of
    {
        template<typename From, typename To>
        struct swap
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<From>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<To>));

            typedef void type;
        };
    }

    template<typename From, typename To>
    typename result_of::swap<From&,To&>::type
    swap(BOOST_FUSION_R_ELSE_LREF(From) from,BOOST_FUSION_R_ELSE_LREF(To) to)
    {
        fusion::for_each(
            fusion::zip(
                BOOST_FUSION_FORWARD(From,from),
                BOOST_FUSION_FORWARD(To,to)),
            detail::swapper());
    }
}}

#endif
