/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_COPY_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_COPY_HPP

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/algorithm/transformation/zip.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

//TODO assoc_copy, assoc_move, assoc_swap

namespace boost { namespace fusion
{
    namespace detail
    {
        struct copier
        {
            template<class ZippedElements>
            void
            operator()(ZippedElements const& zipped_elements)const
            {
                fusion::back(zipped_elements)=fusion::front(zipped_elements);
            }
        };
    }

    namespace result_of
    {
        template<typename From, typename To>
        struct copy
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<From>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<From>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<To>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<To>));

            typedef void type;
        };
    }

    template<typename From, typename To>
    typename result_of::copy<BOOST_FUSION_R_ELSE_CLREF(From),To&>::type
    copy(BOOST_FUSION_R_ELSE_CLREF(From) from,To& to)
    {
        fusion::for_each(
            fusion::zip(BOOST_FUSION_FORWARD(From,from),to),
            detail::copier());
    }
}}

#endif
