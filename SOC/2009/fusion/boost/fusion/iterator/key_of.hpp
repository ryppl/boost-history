/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_KEY_OF_HPP
#define BOOST_FUSION_ITERATOR_KEY_OF_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename>
        struct key_of_impl;
    }

    namespace result_of
    {
        template<typename It>
        struct key_of
          : extension::key_of_impl<typename traits::tag_of<It>::type>::
                template apply<It>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>))
            BOOST_FUSION_MPL_ASSERT((traits::is_associative<It>))
        };
    }
}}

#endif
