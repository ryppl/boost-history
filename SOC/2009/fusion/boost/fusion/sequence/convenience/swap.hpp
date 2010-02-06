/*=============================================================================
    Copyright (c) 2010 Christopher Schmidts

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_SWAP_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_SWAP_HPP

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/algorithm/transformation/zip.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/ref.hpp>
#endif
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
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<From>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<To>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<To>));

            typedef void type;
        };
    }

    template<typename From, typename To>
    typename result_of::swap<From&,To&>::type
    swap(BOOST_FUSION_R_ELSE_LREF(From) from,BOOST_FUSION_R_ELSE_LREF(To) to)
    {
        fusion::for_each(
            fusion::zip(
#ifdef BOOST_NO_RVALUE_REFERENCES
                boost::ref(from),
                boost::ref(to)
#else
                std::forward<From>(from),
                std::forward<To>(to)
#endif
                ),
            detail::swapper());
    }
}}

#endif
