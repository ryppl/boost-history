/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_MOVE_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_MOVE_HPP

#include <boost/fusion/support/internal/base.hpp>
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#   error "Your compiler must support rvalue reference in order to support moving."
#endif

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct mover
        {
            template<class ZippedElements>
            void
            operator()(ZippedElements const& zipped_elements)const
            {
                fusion::back(zipped_elements)=
                    std::move(fusion::front(zipped_elements));
            }
        };
    }

    namespace result_of
    {
        template<typename From, typename To>
        struct move
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<From>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<From>))
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<To>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<To>))

            typedef void type;
        };
    }

    template<typename From, typename To>
    typename result_of::move<From&,To&>::type
    move(From& from,To& to)
    {
        fusion::for_each(fusion::zip(from,to),detail::mover());
    }
}}

#endif
